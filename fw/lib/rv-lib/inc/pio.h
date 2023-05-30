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

// These values match the convention used in the PIO direction register for "bidir" type, as well as our tristate GPIO logic
typedef enum {
    PIO_DIR_IN = 0, // Input
    PIO_DIR_OUT = 1 // Output

} PIO_DIR_ENUM;

//*******************************************
// Defines
//*******************************************
#define GPIO_NUM_MIN 0
#define GPIO_NUM_MAX 35

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

extern PIO_REGS_STRUCT* pio_ledr;           // Output
extern PIO_REGS_STRUCT* pio_sw;             // Input
extern PIO_REGS_STRUCT* pio_key;            // Input
extern PIO_REGS_STRUCT* pio_hex5_hex4;      // Output
extern PIO_REGS_STRUCT* pio_hex3_hex0;      // Output

extern PIO_REGS_STRUCT* pio_gpio_a;         // InOut
extern PIO_REGS_STRUCT* pio_gpio_a_dir;     // Output
extern PIO_REGS_STRUCT* pio_gpio_b;         // InOut
extern PIO_REGS_STRUCT* pio_gpio_b_dir;     // Output
extern PIO_REGS_STRUCT* pio_ard_gpio;       // InOut
extern PIO_REGS_STRUCT* pio_ard_gpio_dir;   // Output

extern PIO_REGS_STRUCT* pio_af_mux_a;       // Output

//*******************************************
// Public Prototypes
//*******************************************

void pio_set_dir(PIO_REGS_STRUCT *pio, uint8_t bit, PIO_DIR_ENUM dir);
void pio_write(PIO_REGS_STRUCT *pio, uint8_t bit, bool val);
void pio_write_port(PIO_REGS_STRUCT *pio, uint32_t port);
void pio_write_port_byte(PIO_REGS_STRUCT *pio, uint8_t byte, uint8_t val);
bool pio_read(PIO_REGS_STRUCT *pio, uint8_t bit);
uint32_t pio_read_port(PIO_REGS_STRUCT *pio);

// For the GPIO[35..0] port
void gpio_set_dir(uint8_t gpio_num, PIO_DIR_ENUM dir);
// For the GPIO[35..0] port
void gpio_write(uint8_t gpio_num, bool val);
// For the GPIO[35..0] port
bool gpio_read(uint8_t gpio_num);

#endif // PIO_H
