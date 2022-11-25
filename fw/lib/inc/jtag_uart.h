// File:    jtag_uart.h
// Purpose: Driver for the Altera JTAG UART peripheral

#ifndef JTAG_UART_H
#define JTAG_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"

//*******************************************
// Defines
//*******************************************

/* Data register */
#define JUART_RAVAIL_SHIFT      16
#define JUART_RAVAIL_MASK       0xFFFF
#define JUART_RVALID            15
#define JUART_DATA              7

/* Control register */
#define JUART_WSPACE_SHIFT      16
#define JUART_WSPACE_MASK       0xFFFF
#define JUART_AC                10
#define JUART_WI                9
#define JUART_RI                8
#define JUART_WE                1
#define JUART_RE                0

//*******************************************
// Structs
//*******************************************

/* Registers for JTAG UART Peripheral */
typedef volatile struct {
    uint32_t data;
    uint32_t control;
} jUartReg;

/* Internal Status of the code module */
typedef struct {
    jUartReg* reg;
    bool pc_connected;
    uint32_t last_ac_set_time;
} jUartStatus;

// juart0
extern jUartReg*    juart0_r;
extern jUartStatus  juart0_s;
extern jUartStatus* juart0;

//*******************************************
// Public Prototypes
//*******************************************

void juart_init(jUartStatus* js, jUartReg* reg);
void juart_serv(jUartStatus* js);
bool juart_is_pc_conn(jUartStatus* js);
uint16_t juart_hw_wspace(jUartStatus* js);

void juart_write(jUartStatus* js, const char* str);
bool juart_put(jUartStatus* js, char c);
char juart_get(jUartStatus* js);

#endif // JTAG_UART_H
