// File:    pio.c
// Purpose: Driver for the Altera PIO peripheral

#include "pio.h"
#include "FpgaConfig.h"
#include "bit.h"

//*******************************************
// Structs
//*******************************************

PIO_REGS_STRUCT* pio_ledr           = (PIO_REGS_STRUCT*)(MEMADDR_PIO_LEDR);
PIO_REGS_STRUCT* pio_sw             = (PIO_REGS_STRUCT*)(MEMADDR_PIO_SW);
PIO_REGS_STRUCT* pio_key            = (PIO_REGS_STRUCT*)(MEMADDR_PIO_KEY);
PIO_REGS_STRUCT* pio_hex5_hex4      = (PIO_REGS_STRUCT*)(MEMADDR_PIO_HEX5_HEX4);
PIO_REGS_STRUCT* pio_hex3_hex0      = (PIO_REGS_STRUCT*)(MEMADDR_PIO_HEX3_HEX0);
PIO_REGS_STRUCT* pio_af_mux_a       = (PIO_REGS_STRUCT*)(MEMADDR_PIO_AF_MUX_A);

//*******************************************
// Functions
//*******************************************

void pio_set_dir(PIO_REGS_STRUCT *pio, uint8_t bit, PIO_DIR_ENUM dir) {
    switch (dir)
    {
    case PIO_DIR_IN:
        BIT_CLR(pio->direction, bit);
        break;
    case PIO_DIR_OUT:
        BIT_SET(pio->direction, bit);
        break;
    default:
        break;
    }
    
    return;
}

void pio_write(PIO_REGS_STRUCT *pio, uint8_t bit, bool val) {
    if (val) {
        BIT_SET(pio->port, bit);
    } else {
        BIT_CLR(pio->port, bit);
    }
}

void pio_write_port(PIO_REGS_STRUCT *pio, uint32_t port) {
    pio->port = port;
}

void pio_write_port_byte(PIO_REGS_STRUCT *pio, uint8_t byte, uint8_t val) {
    WRITE_BYTE(pio->port, byte, val);
}

bool pio_read(PIO_REGS_STRUCT *pio, uint8_t bit) {
    return BIT_TST(pio->port, bit);
}

uint32_t pio_read_port(PIO_REGS_STRUCT *pio) {
    return pio->port;
}