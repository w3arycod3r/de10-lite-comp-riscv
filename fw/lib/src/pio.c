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
PIO_REGS_STRUCT* pio_gpio_a         = (PIO_REGS_STRUCT*)(MEMADDR_PIO_GPIO_A);
PIO_REGS_STRUCT* pio_gpio_a_dir     = (PIO_REGS_STRUCT*)(MEMADDR_PIO_GPIO_A_DIR);
PIO_REGS_STRUCT* pio_gpio_b         = (PIO_REGS_STRUCT*)(MEMADDR_PIO_GPIO_B);
PIO_REGS_STRUCT* pio_gpio_b_dir     = (PIO_REGS_STRUCT*)(MEMADDR_PIO_GPIO_B_DIR);
PIO_REGS_STRUCT* pio_ard_gpio       = (PIO_REGS_STRUCT*)(MEMADDR_PIO_ARD_GPIO);
PIO_REGS_STRUCT* pio_ard_gpio_dir   = (PIO_REGS_STRUCT*)(MEMADDR_PIO_ARD_GPIO_DIR);
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

/*  Mapping the GPIO port to GPIO_A and GPIO_B PIO ports:

    Map GPIO[31..0]  -> GPIO_A[31..0]
    Map GPIO[35..32] -> GPIO_B[3..0]
*/

void gpio_set_dir(uint8_t gpio_num, PIO_DIR_ENUM dir) {
    if (gpio_num < 32) {
        pio_write(pio_gpio_a_dir, gpio_num, (bool)dir);
    } else {
        pio_write(pio_gpio_b_dir, gpio_num-32, (bool)dir);
    }
}

void gpio_write(uint8_t gpio_num, bool val) {
    if (gpio_num < 32) {
        pio_write(pio_gpio_a, gpio_num, val);
    } else {
        pio_write(pio_gpio_b, gpio_num-32, val);
    }
}

bool gpio_read(uint8_t gpio_num) {
    if (gpio_num < 32) {
        return pio_read(pio_gpio_a, gpio_num);
    } else {
        return pio_read(pio_gpio_a, gpio_num-32);
    }
}
