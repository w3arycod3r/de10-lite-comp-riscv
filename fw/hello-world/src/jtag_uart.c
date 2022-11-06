#include "jtag_uart.h"
#include "bit.h"
#include "Hal.h"

JtagUart_Status uart_status;

void UartWriteInt(JtagUart* pUart, int32_t i, bool newline) {

	// An 32 bit integer has at most 10 digits + sign + zero byte
	char buffer[12] = {0};

	// To prevent overflow, handle this as special case
	if (i == INT32_MIN) {

		buffer[0] = '-';
		buffer[1] = '2';
		buffer[2] = '1';
		buffer[3] = '4';
		buffer[4] = '7';
		buffer[5] = '4';
		buffer[6] = '8';
		buffer[7] = '3';
		buffer[8] = '6';
		buffer[9] = '4';
		buffer[10] = '8';
		buffer[11] = 0;

	} else {

		uint8_t bufferIndex = 0;

		if (i < 0) {
			i = -i;
			buffer[bufferIndex++] = '-';
		}

		int32_t temp = i / 10;

		while (temp) {
			temp /= 10;
			bufferIndex++;
		}

		do {
			buffer[bufferIndex--] = (i % 10) + '0';
			i /= 10;
		} while(i);

	}

	UartWrite(pUart, buffer);

	if (newline) {
		UartWrite(pUart, "\n");
	}

}

void UartWriteHex32(JtagUart* pUart, uint32_t ui, bool newline) {
	UartWriteHex8(pUart, (ui>>24) & 0xFF, false);
	UartWriteHex8(pUart, (ui>>16) & 0xFF, false);
	UartWriteHex8(pUart, (ui>>8) & 0xFF, false);
	UartWriteHex8(pUart, (ui) & 0xFF, newline);
}

void UartWriteHex8(JtagUart* pUart, uint8_t byte, bool newline) {
	uint8_t l = byte & 0x0F;
	uint8_t h = (byte >> 4) & 0x0F;
	l = (l < 0x0A) ? l + '0' : l + 'A' - 10;
	h = (h < 0x0A) ? h + '0' : h + 'A' - 10;
	UartPut(pUart, h);
	UartPut(pUart, l);
	if (newline) {
		UartWrite(pUart, "\n");
	}
}

void UartWrite(JtagUart* pUart, const char* str) {
	while (*str) {
		if (*str == '\n') UartPut(pUart, '\r');
		UartPut(pUart, *str++);
	}
}

void UartPut(JtagUart* pUart, char c) {
	uint32_t control = pUart->control;

	// There is space in write buffer
	if (((control >> UART_WSPACE_SHIFT) & UART_WSPACE_MASK)) {
		WRITE_BYTE(pUart->data, 0, c);
	// No space
	} else {
		return;
	}
}

char UartGet(JtagUart* pUart) {
	// Read data register
	uint32_t data = pUart->data;

	// Valid character recv'd
	if (BIT_TST(data, UART_RVALID)){
		return (char)data;
	// Nothing to get
	} else {
		return '\0';
	}

}

#define AC_TMR_TIMEOUT_MSEC

void UartMonitorPC(JtagUart* pUart) {

	// Clear the AC bit (by writing 1)
	BIT_SET(pUart->control, UART_AC);
	uart_status.last_ac_set_time = Hal_ReadTime32();

	// Monitor it to see if PC clears it

	// Timeout when PC takes too long -- assume PC is not connected
	// Like a watchdog timer

	// Restart timed test
}

void UartInit() {
	// Clear the AC bit (by writing 1)
	// BIT_SET(pUart->control, UART_AC);
	uart_status.last_ac_set_time = Hal_ReadTime32();

	uart_status.pc_connected = true;
}