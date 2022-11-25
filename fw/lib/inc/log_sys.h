// File:    log_sys.h
// Purpose: Logging system

#ifndef LOG_SYS_H
#define LOG_SYS_H

#include <stdint.h>
#include <stdbool.h>
#include "FpgaConfig.h"
#include "jtag_uart.h"
#include "uart.h"

//*******************************************
// Defines
//*******************************************


//*******************************************
// Structs
//*******************************************


//*******************************************
// Public Prototypes
//*******************************************

void log_init();
void log_write(const char* str);
void log_put(char c);
void log_serv();

#endif // LOG_SYS_H
