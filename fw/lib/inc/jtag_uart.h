#ifndef JTAG_UART_H
#define JTAG_UART_H

#include <stdint.h>
#include <stdbool.h>

/* Data register */
#define UART_RAVAIL_SHIFT      16
#define UART_RAVAIL_MASK    0xFFFF
#define UART_RVALID         15
#define UART_DATA           7

/* Control register */
#define UART_WSPACE_SHIFT      16
#define UART_WSPACE_MASK  0xFFFF
#define UART_AC          10
#define UART_WI          9
#define UART_RI          8
#define UART_WE          1
#define UART_RE          0

/* Registers for JTAG UART Peripheral */
typedef volatile struct {
    uint32_t data;
    uint32_t control;
} jUartPeriph;

/* Internal Status of the code module */
typedef struct {
    jUartPeriph* reg;
    bool pc_connected;
    uint32_t last_ac_set_time;
} jUartStatus;

void juart_init(jUartStatus* js, jUartPeriph* reg);
void juart_serv(jUartStatus* js);
bool juart_is_pc_conn(jUartStatus* js);
uint8_t juart_hw_wspace(jUartStatus* js);

void juart_write(jUartStatus* js, const char* str);
bool juart_put(jUartStatus* js, char c);
char juart_get(jUartStatus* js);

#endif // JTAG_UART_H
