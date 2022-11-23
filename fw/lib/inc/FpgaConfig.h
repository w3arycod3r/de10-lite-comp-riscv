#ifndef FPGACONFIG_H
#define FPGACONFIG_H

#include <stdint.h>

#include "jtag_uart.h"

#define MHZ                   *1000000UL
#define CPU_CLK_FREQ          (100 MHZ)
#define MSEC_TO_TICKS(msec)   (msec * (CPU_CLK_FREQ / 1000))

/* Peripheral Base Memory Addresses
 * Pulled from Computer_System.html
*/
#define MEMADDR_OCRAM               ((uintptr_t)(0x80000000))
#define MEMADDR_IRQCONTROLLER       ((uintptr_t)(0xff204040))
#define MEMADDR_JTAG_UART_0         ((uintptr_t)(0xff201000))
#define MEMADDR_UART_0              ((uintptr_t)(0xff204080))
#define MEMADDR_PIO_LEDR            ((uintptr_t)(0xff200000))
#define MEMADDR_PIO_SW              ((uintptr_t)(0xff200040))
#define MEMADDR_PIO_KEY             ((uintptr_t)(0xff200050))
#define MEMADDR_PIO_HEX5_HEX4       ((uintptr_t)(0xff200030))
#define MEMADDR_PIO_HEX3_HEX0       ((uintptr_t)(0xff200020))
#define MEMADDR_PIO_ARD_GPIO        ((uintptr_t)(0xff200100))
#define MEMADDR_PIO_ARD_GPIO_DIR    ((uintptr_t)(0xff2040c0))
#define MEMADDR_PIO_GPIO_A          ((uintptr_t)(0xff200060))
#define MEMADDR_PIO_GPIO_A_DIR      ((uintptr_t)(0xff204090))
#define MEMADDR_PIO_GPIO_B          ((uintptr_t)(0xff200070))
#define MEMADDR_PIO_GPIO_B_DIR      ((uintptr_t)(0xff2040a0))
#define MEMADDR_PIO_AF_MUX_A        ((uintptr_t)(0xff2040b0))

// AF Mux bits in GPIO_AF_MUX_A

/*  afm.a.0
    bit = 0 -> GPIO[9] connected to GPIO_A[9]
    bit = 1 -> GPIO[9] connected to UART0_TX
*/
#define AF_MUX_A_UART0_TX   0
/*  afm.a.1
    bit = 0 -> master_reset connected to ~KEY[0]
    bit = 1 -> master_reset connected to '0' (inactive)
*/
#define AF_MUX_A_RST_KEY0   1

#define NUM_LEDR 10

// IRQ Numbers
#define EXT_IRQ_ACCEL           0
#define EXT_IRQ_INT_TMR0        3
#define EXT_IRQ_INT_TMR1        4
#define EXT_IRQ_JTAG_UART       5
#define EXT_IRQ_PIO_ARDUINO     1
#define EXT_IRQ_PIO_JP1_0       2
#define EXT_IRQ_PIO_JP1_1       7
#define EXT_IRQ_PIO_KEY         6

// Build variables, defined by the Makefile at compile time
#ifndef BUILD_VERSION
#define BUILD_VERSION "git undefined"
#endif
#ifndef BUILD_DATE
#define BUILD_DATE "date undefined"
#endif
#ifndef BUILD_STRING
#define BUILD_STRING "build string undefined"
#endif


/* InterruptController

    The InterruptController serves as a simple replacement for the PLIC/CLINT.
    It provides handling for external, soft and timer interrupts.
    External interrupts are connected via Qsys and assert the IRQ_M_EXT signal to the
    core when a bit in both <pending> and <enabled> is high.
    <pending> can be read anytime and shows all current asserted interrupts,
    even if the interrupt is not enabled.
    <mtimeh_latch> is set to the current <mtimeh> value whenever <mtime> (low) is read,
    this is used to snapshot the 64-bit register and read it with 32-bit access without any changes.
    Similary mtimecmp can be written with a 64-bit value at once.
    First <mtimecmph_latch> is written with bits 63 to 32, then <mtimecmp_latch> is written
    with bit 31 to 0. Writing <mtimecmp_latch> writes directly to <mtimecmp>
    and copies the 32-bits of the high-register in the same cycle.
    While mtime is greater or equal to mtimecmp, the IRQ_M_TMR signal is asserted.
    Finally while the register <softinterupt> is holding a value equal nonzero,
    the IRQ_M_SFT signal is asserted. It takes approx. 4 instructions after the write
    until the core is interrupted.

*/

typedef volatile struct {
    uint32_t pending; // R
    uint32_t enabled; // RW
    uint32_t mtime; // RW
    uint32_t mtimeh; // RW
    uint32_t mtimeh_latch; // R
    uint32_t mtimecmp; // RW
    uint32_t mtimecmph; // RW
    uint32_t mtimecmp_latch; // W
    uint32_t mtimecmph_latch; // RW
    uint32_t softinterrupt; // RW
} InterruptController;

extern InterruptController* g_InterruptController;

#endif // FPGACONFIG_H
