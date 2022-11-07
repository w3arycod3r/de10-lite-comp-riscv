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

typedef volatile struct {
  uint32_t data;
  uint32_t control;
} JtagUart;

/* Internal Status */
typedef struct {
  bool pc_connected;
  uint32_t last_ac_set_time;
} JtagUart_Status;

void UartInit();
void UartMonitorPC(JtagUart* pUart);

void UartWriteInt(JtagUart* pUart, int32_t i, bool newline);
void UartWriteHex32(JtagUart* pUart, uint32_t ui, bool newline);
void UartWriteHex8(JtagUart* pUart, uint8_t byte, bool newline);
void UartWrite(JtagUart* pUart, const char* str);

void UartPut(JtagUart* pUart, char c);
char UartGet(JtagUart* pUart);

#endif // Uart_H
