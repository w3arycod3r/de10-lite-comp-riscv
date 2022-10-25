#include "jtag_uart.h"

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
	while (((pUart->control >> UART_WSPACE_SHIFT) & UART_WSPACE_MASK) == 0);
	pUart->data = (uint32_t)c;
}

char UartGet(JtagUart* pUart) {
	while (((pUart->data >> UART_RAVAIL_SHIFT) & UART_RAVAIL_MASK) == 0);
	return (char)(pUart->data & 0xFF);
}

bool UartGetNonBlocking(JtagUart* pUart, char* c) {
	if (((pUart->data >> UART_RAVAIL_SHIFT) & UART_RAVAIL_MASK) != 0) {
		*c = (char)(pUart->data & 0xFF);
		return true;
	}
	return false;
}
