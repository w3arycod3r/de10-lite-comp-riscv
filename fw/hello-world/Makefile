# Makefile for hello-world app
# Uses RISC-V toolchain and Quartus

######################################
# Build Option Variables
######################################
# Optimization Level
OPT_LVL = -Os

PROG_NAME := hello-world

# Common Directories
LIB_DIR = ../lib
TOOLS_DIR = ../tools

######################################
# C Sources
######################################
C_SRC = \
src/Main.c \
$(LIB_DIR)/src/FpgaConfig.c \
$(LIB_DIR)/src/Hal.c \
$(LIB_DIR)/src/jtag_uart.c \
$(LIB_DIR)/src/uart.c \
$(LIB_DIR)/src/pio.c \
$(LIB_DIR)/src/seg7.c \
$(LIB_DIR)/src/multi_button.c \
$(LIB_DIR)/src/log_sys.c \

######################################
# ASM Sources
######################################
A_SRC = \
$(LIB_DIR)/src/Crt.S \
$(LIB_DIR)/src/Hal.S \

######################################
# Build Info
######################################
BUILD_VERSION := $(shell git describe --abbrev=8 --dirty --always --tags)
BUILD_BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
BUILD_COMMIT_NUM := $(shell git rev-list HEAD --count)
BUILD_PWD := $(shell pwd)
BUILD_DATE := $(shell date '+%m-%d-%Y %H:%M:%S %:::z')
BUILD_STRING := $(PROG_NAME)-$(BUILD_VERSION)-$(BUILD_BRANCH).$(BUILD_COMMIT_NUM)

######################################
# Build Directories
######################################
BUILD_DIR = build
# Object files (.o, .S.o)
OBJ_DIR := $(BUILD_DIR)/obj
# Final build files: .hex, .elf, .mif
OUT_DIR := $(BUILD_DIR)/out
# Dependency files: .d
DEP_DIR := $(BUILD_DIR)/dep
# Listing files: .lst
LST_DIR := $(BUILD_DIR)/lst

DIRS := $(OBJ_DIR) $(OUT_DIR) $(DEP_DIR) $(LST_DIR)

######################################
# Output Files
######################################
TARGET = bootrom

### Quartus
QUART_PROJ := DE10_Lite_Computer
QUART_PATH := ../../quartus
QUART_PATH_PROJ := $(QUART_PATH)/$(QUART_PROJ)
QUART_MIF := $(QUART_PATH)/$(TARGET).mif
QUART_SOF := $(QUART_PATH_PROJ).sof
QUART_POF := $(QUART_PATH_PROJ).pof

### Outputs
TARGET_DIR := $(OUT_DIR)/$(TARGET)
DUMP := $(TARGET_DIR).dump
ELF  := $(TARGET_DIR).elf
BIN  := $(TARGET_DIR).bin
HEX  := $(TARGET_DIR).hex
SREC := $(TARGET_DIR).s37
MIF  := $(TARGET_DIR).mif
MAP  := $(TARGET_DIR).map

######################################
# Object Files
######################################
C_OUT := $(addprefix $(OBJ_DIR)/, $(notdir $(C_SRC:.c=.o)))
vpath %.c $(sort $(dir $(C_SRC)))
A_OUT := $(addprefix $(OBJ_DIR)/, $(notdir $(A_SRC:.S=.S.o)))
vpath %.S $(sort $(dir $(A_SRC)))

######################################
# Executables
######################################
CROSS_COMPILE = riscv64-unknown-elf-

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)gcc -x assembler-with-cpp
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

BIN2MIF := python3 $(TOOLS_DIR)/bin2mif.py
RM := rm -rf
COPY := cp

######################################
# CFLAGS / ASFLAGS
######################################

### C Defines
C_DEFS :=  \
-DBUILD_VERSION=\""$(BUILD_VERSION)"\"		\
-DBUILD_DATE=\""$(BUILD_DATE)"\"   			\
-DBUILD_STRING=\""$(BUILD_STRING)"\"   		\

### AS Defines
AS_DEFS =

### C Includes
C_INCLUDES := 	\
-I. 			\
-Iinc 			\
-I$(LIB_DIR)/inc 	\

### AS Includes
AS_INCLUDES =

### RISC-V Architecture / ABI
ARCH := -march=rv32imafc
ABI := -mabi=ilp32f

CFLAGS = $(C_DEFS) $(C_INCLUDES) $(ARCH) $(ABI) $(OPT_LVL) -ffreestanding -fno-delete-null-pointer-checks -g -Wall -Werror -std=gnu99  -ffunction-sections -fdata-sections -flto -fno-strict-aliasing

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(DEP_DIR)/$(notdir $(@:.o=.d))"

ASFLAGS = $(AS_DEFS) $(AS_INCLUDES) $(ARCH) $(ABI) $(OPT_LVL) -ffreestanding -fno-delete-null-pointer-checks -g -Wall -Werror -std=gnu99  -ffunction-sections -fdata-sections -flto -fno-strict-aliasing

######################################
# LDFLAGS (Linker)
######################################

# Linker Script
LD_SCRIPT = link.ld

LDFLAGS := $(ARCH) $(ABI) -T $(LD_SCRIPT) -static -nostartfiles -lc -lgcc -Wl,-print-memory-usage -Wl,-gc-sections -Wl,-Map=$(MAP),--cref

######################################
# Default Target
######################################
all: $(MIF) $(HEX)

######################################
# Build Targets
######################################

make-dirs:
	$(shell mkdir -p $(DIRS))

### Compile C Sources
$(OBJ_DIR)/%.o: %.c Makefile | make-dirs
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(LST_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

### Compile asm sources
$(OBJ_DIR)/%.S.o: %.S Makefile | make-dirs
	$(AS) -c $(ASFLAGS) $< -o $@

### Object file dependencies
-include $(wildcard $(DEP_DIR)/*.d)

### Linker Step
$(ELF): $(C_OUT) $(A_OUT) $(LD_SCRIPT) Makefile
	$(CC) $(A_OUT) $(C_OUT) -o $@ $(LDFLAGS)
	$(OBJDUMP) -D $@ > $(DUMP)

# .elf to .srec
$(SREC): $(ELF)
	$(OBJCOPY) -I elf32-littleriscv -O srec --srec-forceS3 $< $@

# .elf to .bin
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# .bin to .mif file
$(MIF): $(BIN)
	$(BIN2MIF) $< 0x0 > $@

# Write Header into .mif file
	echo -e "-- Build String:\t$(BUILD_STRING)" > $@.hdr.tmp
# echo -e "-- Program:\t\t\t$(PROG_NAME)" >> $@.hdr.tmp
# echo -e "-- Build Version:\t$(BUILD_VERSION)" >> $@.hdr.tmp
# echo -e "-- Branch:\t\t\t$(BUILD_BRANCH).$(BUILD_COMMIT_NUM)" >> $@.hdr.tmp
	echo -e "-- Build Date:\t\t$(BUILD_DATE)" >> $@.hdr.tmp
	@mv $@ $@.tmp
	@cat $@.hdr.tmp $@.tmp > $@
	@rm $@.hdr.tmp $@.tmp

# .elf to .hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

######################################
# Quartus Targets
######################################

# Copy MIF to the Quartus directory
# Make symbols: 
# $@ = Target, $< = Dependency
$(QUART_MIF): $(MIF)
	$(COPY) $< $@

# .mif to .sof and .pof
$(QUART_SOF) $(QUART_POF): $(QUART_MIF)
	quartus_cdb $(QUART_PATH_PROJ) -c $(QUART_PATH_PROJ) --update_mif
	quartus_asm --read_settings_files=on --write_settings_files=off $(QUART_PATH_PROJ) -c $(QUART_PATH_PROJ)

# Temp Flash Target
flash: $(QUART_SOF)
	quartus_pgm -m jtag -o "p;$(QUART_SOF)"

# Perm Flash Target
perm-flash: $(QUART_POF)
	quartus_pgm -m jtag -o "p;$(QUART_POF)"

# Open Terminal Target
term:
	nios2-terminal

# Clean Target
clean:
	$(RM) $(BUILD_DIR)
	$(RM) $(QUART_MIF) $(QUART_SOF) $(QUART_POF)

# Print target -- for debugging Makefile
print:
	@echo C_SRC $(C_SRC)
	@echo A_SRC $(A_SRC)
	@echo DIRS $(DIRS)
	@echo BUILD_PWD $(BUILD_PWD)

# Phony Targets (those which produce no output file)
.PHONY: all clean make-dirs print term flash perm-flash