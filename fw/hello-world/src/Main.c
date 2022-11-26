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
#include "multi_button.h"
#include <string.h>
#include "jtag_uart.h"
#include "uart.h"
#include "log_sys.h"

static int counterMod = 1;

MULTIBUTTON_T  KEY_ARR[2];
MULTIBUTTON_T* KEY    [2];

void IRQHandlerTimer(void) {
    // Invert direction of counter
    counterMod = -counterMod;
    // Rearm timer
    Hal_TimerStart(MSEC_TO_TICKS(2000)); // 2 seconds
}

int main() {

    KEY[0] = &KEY_ARR[0];
    KEY[1] = &KEY_ARR[1];

    mb_init(KEY[0]);
    mb_init(KEY[1]);

    // Init seg7 module
    __seg7_init();

    // LEDs off
    pio_write_port(pio_ledr, 0);

    // Enable external and timer interrupts
    Hal_EnableMachineInterrupt(IRQ_M_EXT);
    Hal_SetTimerIrqHandler(IRQHandlerTimer);
    Hal_TimerStart(MSEC_TO_TICKS(3000)); // 3 seconds
    Hal_GlobalEnableInterrupts();
    Hal_SysTickInit();

    // This should be done after ext irq's are set up, since this sets up the UART's
    log_init();

    // // Determine params of uart0
    // char strBuff[64];
    // strcpy(strBuff, "uart0 wspace = ");
    // __seg7_i32ToDecStrCat((int32_t)uart_hw_wspace(uart0), strBuff);
    // strcat(strBuff, "\n");
    // juart_write(juart0, strBuff);

    // // Stall
    // while (1) { }

    // Greetings
    log_write("\n\n* * * VexRiscv Demo - ");
    log_write(BUILD_STRING);
    log_write(" - ");
    log_write(BUILD_DATE);
    log_write(" * * *\n");

    seg7_writeStringPadSpace("Hello World...");
    seg7_writeStringPadSpace(BUILD_STRING);

    uint32_t timeLast = Hal_ReadTime32();
    uint32_t pattern = 1;
    bool dir = false; // false--left, true--right
    uint8_t u8_cntr = 0;
    int8_t i8_cntr = 0;
    uint16_t u16_cntr = 0;
    uint32_t last_sw = pio_read_port(pio_sw);
    uint32_t sw;

    while (1)
    {
        uint32_t timeNow = Hal_ReadTime32();
        uint8_t c;

        // Enable/Disable reset on KEY0
        // SW[0] UP   -> Reset inactive
        // SW[0] DOWN -> Reset tied to KEY0
        pio_write(pio_af_mux_a, AF_MUX_A_RST_KEY0, pio_read(pio_sw, 0));

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
            // pio_write_port(g_pio_ledr, pattern);
        }

        // Main task of the seg7 module
        __seg7_service();

        log_serv();
        Hal_SysTickServ();
        // Display connected status on LEDR0
        // pio_write(g_pio_ledr, 0, juart_is_pc_conn(&juart0));

        // Echo chars on the JTAG UART
        c = juart_get(juart0);
        if (c == '\r') { c = '\n'; }
        if (c) { log_put(c); }

        // Echo chars on the UART
        c = uart_get(uart0);
        if (c == '\r') { c = '\n'; }
        if (c) { log_put(c); }

        pio_write_port(pio_ledr, pio_read_port(pio_sw));

        // Log changes on the switches
        sw = pio_read_port(pio_sw);
        if (sw != last_sw) {
            last_sw = sw;

            // Format
            char strBuff[64];
            strcpy(strBuff, "SW event @ SysTick ");
            __seg7_i32ToDecStrCat((int32_t)Hal_SysTickRead(), strBuff);
            strcat(strBuff, ": SW = ");
            __seg7_u32ToBinStrCat(sw, 10, strBuff);
            strcat(strBuff, "\n");
            // Print
            log_write(strBuff);
        }

        // Read KEY0 and KEY1 events
        for (int i = 0; i < 2; i++)
        {
            MB_EVENT_T key_event = mb_get_event(KEY[i], pio_read(pio_key, i));

            char strBuff[64];
            strcpy(strBuff, "Key ");
            __seg7_i32ToDecStrCat((int32_t)i, strBuff);
            strcat(strBuff, " event @ SysTick ");
            __seg7_i32ToDecStrCat((int32_t)Hal_SysTickRead(), strBuff);

            // Print events
            switch (key_event)
            {
            case MB_EVENT_CLICK:
                strcat(strBuff, ": MB_EVENT_CLICK\n");
                log_write(strBuff);
                break;
            case MB_EVENT_DOUBLE_CLICK:
                strcat(strBuff, ": MB_EVENT_DOUBLE_CLICK\n");
                log_write(strBuff);
                break;
            case MB_EVENT_HOLD:
                strcat(strBuff, ": MB_EVENT_HOLD\n");
                log_write(strBuff);
                break;
            case MB_EVENT_LONG_HOLD:
                strcat(strBuff, ": MB_EVENT_LONG_HOLD\n");
                log_write(strBuff);
                break;
            
            default:
                break;
            }

            // Handle Counters
            // Key 0 click -> Increment
            if ((i == 0) && (key_event == MB_EVENT_CLICK))
            {
                u8_cntr += 1;
                i8_cntr += 1;
                u16_cntr += 1;
            }
            // Key 0 double click -> Fast Increment
            if ((i == 0) && (key_event == MB_EVENT_DOUBLE_CLICK))
            {
                u8_cntr += 100;
                i8_cntr += 100;
                u16_cntr += 100;
            }
            // Key 0 hold -> Clear
            if ((i == 0) && (key_event == MB_EVENT_HOLD))
            {
                u8_cntr = 0;
                i8_cntr = 0;
                u16_cntr = 0;
            }
            // Key 1 click -> Decrement
            if ((i == 1) && (key_event == MB_EVENT_CLICK))
            {
                u8_cntr -= 1;
                i8_cntr -= 1;
                u16_cntr -= 1;
            }
            // Key 1 double click -> Fast Decrement
            if ((i == 1) && (key_event == MB_EVENT_DOUBLE_CLICK))
            {
                u8_cntr -= 100;
                i8_cntr -= 100;
                u16_cntr -= 100;
            }
            
        }

        // Control 7-seg mode

        // SW[9] controls blink
        seg7_blink(pio_read(pio_sw, 9));

        sw = pio_read_port(pio_sw);

        // SW[3..1] control what is displayed
        switch ((sw >> 1) & 0b111)
        {
        case 0b000:
            seg7_writeStringPadSpace(BUILD_STRING);
            break;
        case 0b001:
            seg7_writeString("bright");
            break;
        case 0b010:
            seg7_writeU8Decimal(u8_cntr);
            break;
        case 0b011:
            seg7_writeU8Hex(u8_cntr);
            break;
        case 0b100:
            seg7_writeI8Decimal(i8_cntr);
            break;
        case 0b101:
            seg7_writeI8Hex(i8_cntr);
            break;
        case 0b110:
            seg7_writeU16Decimal(u16_cntr);
            break;
        case 0b111:
            seg7_writeU16Hex(u16_cntr);
            break;
        
        default:
            break;
        }
    }
}
