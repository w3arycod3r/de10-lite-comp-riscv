#ifndef PIO_H
#define PIO_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"

typedef enum {
    PIO_DIR_IN,
    PIO_DIR_OUT

} PIO_DIR_T;

void pio_set_dir(Pio *pPio, uint8_t bit, PIO_DIR_T dir);
void pio_write(Pio *pPio, uint8_t bit, bool val);
void pio_write_port(Pio *pPio, uint32_t port);
void pio_write_port_byte(Pio *pPio, uint8_t byte, uint8_t val);
bool pio_read(Pio *pPio, uint8_t bit);
uint32_t pio_read_port(Pio *pPio);

#endif // PIO_H
