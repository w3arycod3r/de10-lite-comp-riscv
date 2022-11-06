#include "FpgaConfig.h"

Pio* g_pio_ledr         = (Pio*)(MEMADDR_PIO_LEDR);
Pio* g_pio_sw           = (Pio*)(MEMADDR_PIO_SW);
Pio* g_pio_key          = (Pio*)(MEMADDR_PIO_KEY);
Pio* g_pio_hex5_hex4    = (Pio*)(MEMADDR_PIO_HEX5_HEX4);
Pio* g_pio_hex3_hex0    = (Pio*)(MEMADDR_PIO_HEX3_HEX0);

JtagUart* g_Uart        = (JtagUart*)(MEMADDR_JTAG_UART);
InterruptController* g_InterruptController = (InterruptController*)(MEMADDR_IRQCONTROLLER);
