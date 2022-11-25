// File:    jtag_uart.c
// Purpose: Driver for the Altera JTAG UART peripheral

#include "jtag_uart.h"
#include "bit.h"
#include "Hal.h"
#include "FpgaConfig.h"

//*******************************************
// Defines
//*******************************************

#define AC_TMR_TIMEOUT_MSEC 500

//*******************************************
// Static Prototypes
//*******************************************
static void juart0_irq_handler();

//*******************************************
// Variables
//*******************************************

// juart0
jUartReg*    juart0_r    = (jUartReg*)(MEMADDR_JTAG_UART_0);
jUartStatus  juart0_s;
jUartStatus* juart0 	 = &juart0_s;

//*******************************************
// Functions
//*******************************************

static void juart0_irq_handler() {
    
}

/* Init the code module and the hardware */
void juart_init(jUartStatus* js, jUartReg* reg) {

	// Save the peripheral pointer
	js->reg = reg;

	// Clear the AC bit (by writing 1)
	BIT_SET(js->reg->control, JUART_AC);

	// Reset PC watchdog
	js->last_ac_set_time = Hal_ReadTime32();

	// Assume PC not connected initially
	js->pc_connected = true;

	// Setup JTAG UART rx and tx interrupts
	// For now, these are not doing any real work
	if (reg == juart0_r) {
		Hal_SetExtIrqHandler(EXT_IRQ_JTAG_UART_0, juart0_irq_handler);
		Hal_EnableInterrupt(EXT_IRQ_JTAG_UART_0);
		BIT_CLR(reg->control, JUART_RE);	// disable read interrupts
		BIT_CLR(reg->control, JUART_WE);	// disable write interrupts
	}

}

/* Service the JTAG UART code module */
void juart_serv(jUartStatus* js) {

	// PC has set the AC bit
	if (BIT_TST(js->reg->control, JUART_AC))
	{
		// A connection is present
		js->pc_connected = true;
		// Reset watchdog timer
		js->last_ac_set_time = Hal_ReadTime32();
		// Clear the AC bit (by writing 1)
		BIT_SET(js->reg->control, JUART_AC);
	}
	

	// Timeout when PC takes too long -- assume PC is not connected
	// Like a watchdog timer
	if (HAL_HAS_DURATION_PASSED_MSEC(js->last_ac_set_time, AC_TMR_TIMEOUT_MSEC))
	{
		js->pc_connected = false;
		js->last_ac_set_time = Hal_ReadTime32();
	}
	
}

bool juart_is_pc_conn(jUartStatus* js) {
	return js->pc_connected;
}

void juart_write(jUartStatus* js, const char* str) {
	bool status;
	while (*str) {
		if (*str == '\n') juart_put(js, '\r');
		status = juart_put(js, *str++);

		// If we drop a character, drop the rest of the string
		if (!status) return;
	}
}

// How much space is left in the write buffer?
uint16_t juart_hw_wspace(jUartStatus* js) {
	return ((js->reg->control >> JUART_WSPACE_SHIFT) & JUART_WSPACE_MASK);
}

bool juart_put(jUartStatus* js, char c) {

	// There is space in write buffer
	if (juart_hw_wspace(js)) {
		WRITE_BYTE(js->reg->data, 0, c);
		return true;
	}

	// If PC is connected, wait for space 
	while (juart_is_pc_conn(js) && !juart_hw_wspace(js)) {
		juart_serv(js);
	}

	// We got space, write the char
	if (juart_hw_wspace(js))
	{
		WRITE_BYTE(js->reg->data, 0, c);
		return true;
	}
	
	// PC disconnected, drop the char
	return false;
}

char juart_get(jUartStatus* js) {
	// Read data register
	uint32_t data = js->reg->data;

	// Valid character recv'd
	if (BIT_TST(data, JUART_RVALID)){
		return (char)data;
	// Nothing to get
	} else {
		return '\0';
	}

}
