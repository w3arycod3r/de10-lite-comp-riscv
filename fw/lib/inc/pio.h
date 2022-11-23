// File:    pio.h
// Purpose: Driver for the Altera PIO peripheral

#ifndef PIO_H
#define PIO_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"

//*******************************************
// Enums
//*******************************************
typedef enum {
    PIO_DIR_IN,
    PIO_DIR_OUT

} PIO_DIR_ENUM;

//*******************************************
// Structs
//*******************************************

typedef volatile struct {
    uint32_t port;
    uint32_t direction; // 0 for input, 1 for output
    uint32_t _reserved1;
    uint32_t _reserved2;
    uint32_t outset;
    uint32_t outclear;
} PIO_REGS_STRUCT;

extern PIO_REGS_STRUCT* pio_ledr;
extern PIO_REGS_STRUCT* pio_sw;
extern PIO_REGS_STRUCT* pio_key;
extern PIO_REGS_STRUCT* pio_hex5_hex4;
extern PIO_REGS_STRUCT* pio_hex3_hex0;
extern PIO_REGS_STRUCT* pio_af_mux_a;

//*******************************************
// Public Prototypes
//*******************************************

void pio_set_dir(PIO_REGS_STRUCT *pio, uint8_t bit, PIO_DIR_ENUM dir);
void pio_write(PIO_REGS_STRUCT *pio, uint8_t bit, bool val);
void pio_write_port(PIO_REGS_STRUCT *pio, uint32_t port);
void pio_write_port_byte(PIO_REGS_STRUCT *pio, uint8_t byte, uint8_t val);
bool pio_read(PIO_REGS_STRUCT *pio, uint8_t bit);
uint32_t pio_read_port(PIO_REGS_STRUCT *pio);

#endif // PIO_H
