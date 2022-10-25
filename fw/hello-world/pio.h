#ifndef PIO_H
#define PIO_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"

void pio_write(Pio *pPio, uint8_t bit, bool val);
bool pio_read(Pio *pPio, uint8_t bit);
void pio_write_port(Pio *pPio, uint32_t port);

#endif // PIO_H
