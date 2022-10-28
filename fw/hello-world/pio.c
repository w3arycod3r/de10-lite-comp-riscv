#include "pio.h"
#include "FpgaConfig.h"
#include "bit.h"

void pio_write(Pio *pPio, uint8_t bit, bool val) {
    if (val) {
        BIT_SET(pPio->port, bit);
    } else {
        BIT_CLR(pPio->port, bit);
    }
}

void pio_write_port(Pio *pPio, uint32_t port) {
    pPio->port = port;
}

void pio_write_port_byte(Pio *pPio, uint8_t byte, uint8_t val) {
    WRITE_BYTE(pPio->port, byte, val);
}

bool pio_read(Pio *pPio, uint8_t bit) {
    return BIT_TST(pPio->port, bit);
}

uint32_t pio_read_port(Pio *pPio) {
    return pPio->port;
}