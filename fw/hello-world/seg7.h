#ifndef SEG_7_H
#define SEG_7_H

#include   <stdbool.h>
#include   <stdint.h>
#include   <string.h>
#include   <stdlib.h>


// ENUMERATIONS
typedef enum { SEG7_MODE_STATIC, SEG7_MODE_SCROLL } SEG7_MODE_T;

// STRUCTURES


// DEFINEs and CONSTANTs
#define __SEG7_NUM_DIGITS 6
#define __SEG7_SCROLL_PER_TICKS MSEC_TO_TICKS(500)
#define __SEG7_BLINK_PER_TICKS  MSEC_TO_TICKS(250)
#define __SEG7_MAX_MSG_SIZE 50

extern const uint8_t __7segfontTable[];


// PRIVATE DATA

// PRIVATE HW FUNCTION PROTOTYPES

// Ones in the pattern where segments should be ON
// bit 7: decimal pt, bits 6-0 : segs G-A
// digit 0-3 : right to left
void    __seg7_hw_set_digit(uint8_t u8_dig, uint8_t u8_pattern);
void    __seg7_hw_config();
void    __seg7_update_displays();

// PRIVATE FUNCTION PROTOTYPES
void    __seg7_init(void);
uint8_t __seg7_char_to_patt(char c_char);
void    __seg7_writeStringRightJust(char* psz_data);
void    __seg7_i32ToDecStr(int32_t i32_value, char* psz_outputStr);
char    __seg7_nibbleToHexChar(uint8_t u8_value);
void    __seg7_u8ToHexStr(uint8_t u8_value, char* psz_outputStr);
void    __seg7_u16ToHexStr(uint16_t u16_value, char* psz_outputStr);
void    __seg7_shiftLeftStr(char* psz_inStr, char c_inChar);
bool    __seg7_hasScrollBuffChanged(uint8_t* pu8_data, uint8_t u8_bufflen);

// Main task to manage the displays
void __seg7_service();

// PUBLIC API FUNCTION PROTOTYPES

void  seg7_clearScreen(void);
void  seg7_writeCharacter(uint8_t u8_column, uint8_t u8_data);
void  seg7_writeBuffer(uint8_t* pu8_data, uint8_t u8_bufflen);
void  seg7_writeString(char* psz_data);
char* seg7_getDisplay(void);
void  seg7_blink(bool b_state);
void  seg7_writeU8Decimal(uint8_t u8_data);
void  seg7_writeU8Hex(uint8_t u8_data);
void  seg7_writeI8Decimal(int8_t i8_data);
void  seg7_writeI8Hex(int8_t i8_data);
void  seg7_writeU16Decimal(uint16_t u16_data);
void  seg7_writeU16Hex(uint16_t u16_data);



#endif    // SEG_7_H