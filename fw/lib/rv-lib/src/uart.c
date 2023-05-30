// File:    uart.c
// Purpose: Driver for the Altera RS232 UART (University Program) peripheral

#include "uart.h"
#include "bit.h"
#include "Hal.h"
#include "FpgaConfig.h"
#include "pio.h"

//*******************************************
// Defines
//*******************************************

//*******************************************
// Static Prototypes
//*******************************************
static void uart0_irq_handler();

//*******************************************
// Variables
//*******************************************

// uart0
UartReg* 	uart0_r    	= (UartReg*)(MEMADDR_UART_0);
UartStatus  uart0_s;
UartStatus* uart0 	 	= &uart0_s;

//*******************************************
// Functions
//*******************************************

static void uart0_irq_handler() {

}

/* Init the code module and the hardware */
void uart_init(UartStatus* us, UartReg* reg) {

	// Save the peripheral pointer
	us->reg = reg;

	// Set init status
	us->init_done = true;

	// uart0 specific init stuff
	if (reg == uart0_r) {
		// Setup UART rx and tx interrupts
		// For now, these are not doing any real work
		Hal_SetExtIrqHandler(EXT_IRQ_UART_0, uart0_irq_handler);
		Hal_EnableInterrupt(EXT_IRQ_UART_0);
		BIT_CLR(reg->control, UART_RE);	// disable read interrupts
		BIT_CLR(reg->control, UART_WE);	// disable write interrupts

		// Configure af mux to route uart0_tx to GPIO[9]
		pio_write(pio_af_mux_a, AF_MUX_A_UART0_TX, 1);

		// Configure GPIO[9] as output
		gpio_set_dir(9, PIO_DIR_OUT);
	}

}

/* Service the UART code module */
void uart_serv(UartStatus* us) {

	
}

void uart_write(UartStatus* us, const char* str) {
	bool status;
	while (*str) {
		if (*str == '\n') uart_put(us, '\r');
		status = uart_put(us, *str++);

		// If we drop a character, drop the rest of the string
		if (!status) return;
	}
}

/* 	How much space is left in the write buffer?
	This peripheral has a 128 byte TX FIFO
*/
uint16_t uart_hw_wspace(UartStatus* us) {
	return ((us->reg->control >> UART_WSPACE_SHIFT) & UART_WSPACE_MASK);
}

bool uart_put(UartStatus* us, char c) {

	// Wait for space
	while (!uart_hw_wspace(us)) { }

	// Write byte
	WRITE_BYTE(us->reg->data, 0, c);
	return true;

}

char uart_get(UartStatus* us) {
	// Read data register
	uint32_t data = us->reg->data;

	// Valid character recv'd
	if (BIT_TST(data, UART_RVALID)){
		return (char)data;
	// Nothing to get
	} else {
		return '\0';
	}

}
