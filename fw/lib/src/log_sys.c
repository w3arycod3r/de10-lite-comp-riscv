// File:    log_sys.c
// Purpose: Logging system

#include "uart.h"
#include "bit.h"
#include "Hal.h"
#include "FpgaConfig.h"
#include "jtag_uart.h"
#include "uart.h"

//*******************************************
// Defines
//*******************************************


//*******************************************
// Variables
//*******************************************

static bool log_sys_init_done = false;

//*******************************************
// Functions
//*******************************************


/* Init the code module and the hardware UART's */
void log_init() {

	juart_init(juart0, juart0_r);
    uart_init(uart0, uart0_r);

	log_sys_init_done = true;

}

/* 	Write a string to the log system.
	Currently, we will just send this string to both of our uart outputs */
void log_write(const char* str) {

	juart_write(juart0, str);
	uart_write(uart0, str);

}

void log_put(char c) {

	juart_put(juart0, c);
	uart_put(uart0, c);

}

/* Service the code module and the hardware UART's */
void log_serv() {

	juart_serv(juart0);
}

