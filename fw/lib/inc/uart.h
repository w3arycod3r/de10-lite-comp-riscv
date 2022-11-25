// File:    uart.h
// Purpose: Driver for the Altera RS232 UART (University Program) peripheral

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"

//*******************************************
// Defines
//*******************************************

/* Data register */
#define UART_RAVAIL_SHIFT       16
#define UART_RAVAIL_MASK        0xFF    // RAVAIL field is 8 bits
#define UART_RVALID             15
#define UART_PE                 9       // Parity Error
#define UART_DATA               7       // Using in 8 data bits mode

/* Control register */
#define UART_WSPACE_SHIFT       16
#define UART_WSPACE_MASK        0xFF    // WSPACE field is 8 bits
#define UART_WI                 9       // Write Interrupt Pending
#define UART_RI                 8       // Read Interrupt Pending
#define UART_WE                 1       // Write Interrupt Enable
#define UART_RE                 0       // Read Interrupt Enable

//*******************************************
// Structs
//*******************************************

/* Registers for UART Peripheral */
typedef volatile struct {
    uint32_t data;
    uint32_t control;
} UartReg;

/* Internal Status of the code module */
typedef struct {
    UartReg* reg;
    bool init_done;
} UartStatus;

// uart0
extern UartReg*    uart0_r;
extern UartStatus  uart0_s;
extern UartStatus* uart0;

//*******************************************
// Public Prototypes
//*******************************************

void uart_init(UartStatus* us, UartReg* reg);
void uart_serv(UartStatus* us);
uint16_t uart_hw_wspace(UartStatus* us);

void uart_write(UartStatus* us, const char* str);
bool uart_put(UartStatus* us, char c);
char uart_get(UartStatus* us);

#endif // UART_H
