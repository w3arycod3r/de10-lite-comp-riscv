/* Copyright (C) 2021 ARIES Embedded GmbH

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE. */

#include <stdint.h>

#include "FpgaConfig.h"
#include "Hal.h"
#include "bit.h"
#include "pio.h"
#include "seg7.h"

static int counterMod = 1;

jUartStatus juart0;

void IRQHandlerTimer(void) {
    // Invert direction of counter
    counterMod = -counterMod;
    // Rearm timer
    Hal_TimerStart(MSEC_TO_TICKS(2000)); // 2 seconds
}

void IRQHandlerUart() {
    
}

int main() {

    juart_init(&juart0, juart0_p);

    // Greetings
    juart_write(&juart0, "\n\n* * * VexRiscv Demo - ");
    juart_write(&juart0, BUILD_STRING);
    juart_write(&juart0, " - ");
    juart_write(&juart0, BUILD_DATE);
    juart_write(&juart0, " * * *\n");

    // Init seg7 module
    __seg7_init();

    // LEDs off
    pio_write_port(g_pio_ledr, 0);

    // Enable interrupt on timer and uart receive.
    Hal_SetExtIrqHandler(EXT_IRQ_JTAG_UART, IRQHandlerUart);
    Hal_EnableInterrupt(EXT_IRQ_JTAG_UART);
    BIT_CLR(juart0.reg->control, UART_RE);	// enable read interrupts
    BIT_CLR(juart0.reg->control, UART_WE);	// disable write interrupts

    Hal_EnableMachineInterrupt(IRQ_M_EXT);
    Hal_SetTimerIrqHandler(IRQHandlerTimer);
    Hal_TimerStart(MSEC_TO_TICKS(3000)); // 3 seconds
    Hal_GlobalEnableInterrupts();

    seg7_writeStringPadSpace("Hello World...");
    seg7_writeStringPadSpace(BUILD_STRING);

    uint32_t timeLast = Hal_ReadTime32();
    uint32_t pattern = 1;
    bool dir = false; // false--left, true--right
    while (1)
    {
        uint32_t timeNow = Hal_ReadTime32();

        // Trigger every 250 msec
        if ((timeNow - timeLast) > (MSEC_TO_TICKS(100))) {
            timeLast = timeNow;

            // Shift
            if (dir) {
                pattern >>= 1;
            } else {
                pattern <<= 1;
            }

            // Flip direction
            if ( (pattern == (1 << (NUM_LEDR-1))) || (pattern == 1) )
            {
                dir = !dir;
            }

            // Drive
            pio_write_port(g_pio_ledr, pattern);
        }

        // Main task of the seg7 module
        __seg7_service();

        juart_serv(&juart0);
        // Display connected status on LEDR0
        pio_write(g_pio_ledr, 0, juart_is_pc_conn(&juart0));

        // Echo chars on the UART
        uint8_t c = juart_get(&juart0);
        if (c == '\r') { c = '\n'; }
        if (c) { juart_put(&juart0, c); }
    }
}
