#include "seg7.h"
#include "pio.h"
#include "FpgaConfig.h"
#include "bit.h"
#include "Hal.h"
#include <string.h>

// CONSTANTS

/*
 *  Project     Segmented LED Display - ASCII Library
 *  @author     David Madison
 *  @link       github.com/dmadison/Segmented-LED-Display-ASCII
 *  @license    MIT - Copyright (c) 2017 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

// 95 printable chars, ASCII range: 32 (space) to 126 (~)
// bit 7 to 0: DP,G-A. Bits are high for ON segments. 
const uint8_t __7segfontTable[] = {
    0x00, /* (space) */
    0x86, /* ! */
    0x22, /* " */
    0x7E, /* # */
    0x6D, /* $ */
    0xD2, /* % */
    0x46, /* & */
    0x20, /* ' */
    0x29, /* ( */
    0x0B, /* ) */
    0x21, /* * */
    0x70, /* + */
    0x10, /* , */
    0x40, /* - */
    0x80, /* . */
    0x52, /* / */
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x09, /* : */
    0x0D, /* ; */
    0x61, /* < */
    0x48, /* = */
    0x43, /* > */
    0xD3, /* ? */
    0x5F, /* @ */
    0x77, /* A */
    0x7C, /* B */
    0x39, /* C */
    0x5E, /* D */
    0x79, /* E */
    0x71, /* F */
    0x3D, /* G */
    0x76, /* H */
    0x30, /* I */
    0x1E, /* J */
    0x75, /* K */
    0x38, /* L */
    0x15, /* M */
    0x37, /* N */
    0x3F, /* O */
    0x73, /* P */
    0x6B, /* Q */
    0x33, /* R */
    0x6D, /* S */
    0x78, /* T */
    0x3E, /* U */
    0x3E, /* V */
    0x2A, /* W */
    0x76, /* X */
    0x6E, /* Y */
    0x5B, /* Z */
    0x39, /* [ */
    0x64, /* \ */
    0x0F, /* ] */
    0x23, /* ^ */
    0x08, /* _ */
    0x02, /* ` */
    0x5F, /* a */
    0x7C, /* b */
    0x58, /* c */
    0x5E, /* d */
    0x7B, /* e */
    0x71, /* f */
    0x6F, /* g */
    0x74, /* h */
    0x10, /* i */
    0x0C, /* j */
    0x75, /* k */
    0x30, /* l */
    0x14, /* m */
    0x54, /* n */
    0x5C, /* o */
    0x73, /* p */
    0x67, /* q */
    0x50, /* r */
    0x6D, /* s */
    0x78, /* t */
    0x1C, /* u */
    0x1C, /* v */
    0x14, /* w */
    0x76, /* x */
    0x6E, /* y */
    0x5B, /* z */
    0x46, /* { */
    0x30, /* | */
    0x70, /* } */
    0x01, /* ~ */
};

// PRIVATE ESOS 7SEG SERVICE VARIABLES (mark volatile)
volatile char __sz_seg7_currDisp[__SEG7_NUM_DIGITS+1];
volatile char __sz_seg7_spaces[__SEG7_NUM_DIGITS+1];
volatile char __sz_seg7_dashes[__SEG7_NUM_DIGITS+1];
volatile char __sz_seg7_msgScroll[__SEG7_MAX_MSG_SIZE+1];
volatile uint8_t __u8_seg7_msgScrollLen;
volatile uint8_t __u8_seg7_scrollIndex;
volatile uint8_t __u8_seg7_mode;
volatile bool __b_seg7_blinkOn;
volatile bool __b_seg7_screenBlank;
volatile uint32_t __u32_seg7_lastBlinkTime;
volatile uint32_t __u32_seg7_lastScrollTime;

/***** PRIVATE 7SEG HARDWARE FUNCTIONS *****/

void __seg7_hw_config ( void )
{	
  	/* These GPIO pins are output only, no need to set direction */
	
	/* Set initial state of the GPIO pins */
    pio_write_port(pio_hex5_hex4, 0);
    pio_write_port(pio_hex3_hex0, 0);
    
}

/* 
   This function sets a pattern to a single display.
   Displays are indexed as so: digit 5-0 -- left to right
   Pattern has active HIGH bits. Ones in the pattern for ON segments.
   Bits are arranged as so: bit 7 -- decimal pt, bits 6-0 -- segs G-A
*/
void __seg7_hw_set_digit(uint8_t u8_dig, uint8_t u8_pattern) {

    switch (u8_dig)
    {
    case 5:
        pio_write_port_byte(pio_hex5_hex4, 1, u8_pattern);
        break;
    case 4:
        pio_write_port_byte(pio_hex5_hex4, 0, u8_pattern);
        break;
    case 3:
        pio_write_port_byte(pio_hex3_hex0, 3, u8_pattern);
        break;
    case 2:
        pio_write_port_byte(pio_hex3_hex0, 2, u8_pattern);
        break;
    case 1:
        pio_write_port_byte(pio_hex3_hex0, 1, u8_pattern);
        break;
    case 0:
        pio_write_port_byte(pio_hex3_hex0, 0, u8_pattern);
        break;

    default:
        break;
    }

}

/***** PRIVATE 7SEG FUNCTIONS *****/

void __seg7_init(void) {

    // Call the HW specific init code
    __seg7_hw_config();

    // Init SW structures
    strcpy((char *)__sz_seg7_spaces, "");
    strcpy((char *)__sz_seg7_dashes, "");
    for (uint8_t i = 0; i < __SEG7_NUM_DIGITS; i++)
    {
        strcat((char*)__sz_seg7_spaces, " ");  // Static spaces string
        strcat((char*)__sz_seg7_dashes, "-");  // Static dashes string
    }
    strcpy((char*)__sz_seg7_currDisp, (char*)__sz_seg7_spaces);
    __sz_seg7_msgScroll[0] = '\0';
    __u8_seg7_msgScrollLen = 0;
    __u8_seg7_scrollIndex = 0;
    __u8_seg7_mode = SEG7_MODE_STATIC;
    __b_seg7_blinkOn = false;
    __b_seg7_screenBlank = false;
    __u32_seg7_lastBlinkTime = Hal_ReadTime32();
    __u32_seg7_lastScrollTime = Hal_ReadTime32();
}

/* 
   For a non multiplexed display -- where all the digits have their own i/o lines.
   A "set-and-forget" interface.
   Sets the displays to show what is currently in the __sz_seg7_currDisp array 
*/
void __seg7_update_displays() {

    for (uint8_t strIndex = 0; strIndex < __SEG7_NUM_DIGITS; strIndex++)
    {
        // String is indexed left to right, whereas digits are indexed right to left
        uint8_t digit = __SEG7_NUM_DIGITS - strIndex - 1;
        uint8_t pattern = __seg7_char_to_patt(__sz_seg7_currDisp[strIndex]);

        // Blanking bit is toggled quickly during "blink" mode
        if (__b_seg7_screenBlank) {
            pattern = 0;
        }

        __seg7_hw_set_digit(digit, pattern);
        
    }

}

// Prints the string right justified on the displays.
// This func is only for strings that fit on the displays without scrolling.
void __seg7_writeStringRightJust(char* psz_data) {
    uint8_t u8_strLen = strlen(psz_data);
    uint8_t u8_begIndex;

    if (u8_strLen > __SEG7_NUM_DIGITS) {
        return;
    }

    // Right justify formula:
    // begIdx = endIdx-len+1
    // endIdx is (__SEG7_NUM_DIGITS-1) ... Simplifies to below expr
    u8_begIndex = (__SEG7_NUM_DIGITS) - u8_strLen;

    // Clear the currDisp buffer
    strcpy((char*)__sz_seg7_currDisp, (char*)__sz_seg7_spaces);

    // Copy incoming string into currDisp starting at begIndex
    strcpy((char*)&__sz_seg7_currDisp[u8_begIndex], psz_data);
    
    __u8_seg7_mode = SEG7_MODE_STATIC;
    __seg7_update_displays();
}

// Check if the new scroll buff we have been passed differs from the current one we're storing.
bool __seg7_hasScrollBuffChanged(uint8_t* pu8_data, uint8_t u8_bufflen) {

    // If the sizes differ, there is a change
    if (u8_bufflen != __u8_seg7_msgScrollLen) {
        return true;
    }

    // New buffer and old buffer are the same size, let's check if any bytes have changed.
    uint8_t* pu8_oldBuffer = (uint8_t*)__sz_seg7_msgScroll;
    uint8_t* pu8_newBuffer = pu8_data;
    bool dataChanged = false;

    for (uint8_t u8_index = 0; u8_index < u8_bufflen; u8_index++) {
        if (pu8_oldBuffer[u8_index] != pu8_newBuffer[u8_index]) {
            dataChanged = true;
            break;
        }
    }

    return dataChanged;
}


/**
* A "hidden" (system task) that manages 7 segment displays.
*
* \hideinitializer
*/
void __seg7_service () {

    // Local Vars
    
    // while(TRUE) {

            /**** Handle scrolling message ****/
        if (__u8_seg7_mode == SEG7_MODE_SCROLL) {

            // Time to scroll in the next char
            if (HAL_HAS_DURATION_PASSED(__u32_seg7_lastScrollTime, __SEG7_SCROLL_PER_TICKS)) {

                char c_nextChar = __sz_seg7_msgScroll[__u8_seg7_scrollIndex];
                __seg7_shiftLeftStr((char*)__sz_seg7_currDisp, c_nextChar);

                // Update the index 
                __u8_seg7_scrollIndex++;
                if (__u8_seg7_scrollIndex >= __u8_seg7_msgScrollLen) {
                    __u8_seg7_scrollIndex = 0;
                }

                // Save the time
                __u32_seg7_lastScrollTime = Hal_ReadTime32();

                // Update Displays
                __seg7_update_displays();
            }
        }

            /**** Handle blinking ****/
        if (__b_seg7_blinkOn) {

            // Time to toggle the "blanking" bit
            if (HAL_HAS_DURATION_PASSED(__u32_seg7_lastBlinkTime, __SEG7_BLINK_PER_TICKS)) {
                __b_seg7_screenBlank = !__b_seg7_screenBlank;
                
                // Save the time
                __u32_seg7_lastBlinkTime = Hal_ReadTime32();

                // Update Displays
                __seg7_update_displays();
            }

        // Blinking was turned off, but "blanking" bit is still active
        } else if (__b_seg7_screenBlank) {

            __b_seg7_screenBlank = false;
            __seg7_update_displays();
        }


    // } // end while(TRUE)

}

//*********************************************
// PUBLIC SEG7 FUNCTIONS (External API)
//*********************************************

void seg7_clearScreen(void) {
    __u8_seg7_mode = SEG7_MODE_STATIC;
    strcpy((char*)__sz_seg7_currDisp, (char*)__sz_seg7_spaces);
    __seg7_update_displays();
}

// Column 0 is the leftmost one
void seg7_writeCharacter(uint8_t u8_column, uint8_t u8_data) {
    if (u8_column < __SEG7_NUM_DIGITS) {
        __sz_seg7_currDisp[u8_column] = u8_data;
        __u8_seg7_mode = SEG7_MODE_STATIC;
    }
    __seg7_update_displays();
}

void seg7_writeBuffer(uint8_t* pu8_data, uint8_t u8_bufflen) {
    // Local Vars
    uint8_t u8_index;
    uint8_t u8_oldMode = __u8_seg7_mode;
    uint8_t u8_newMode;

    if (u8_bufflen == 0) {
        return;
    }

    // Clip the length if it is over the max storage size
    if (u8_bufflen > __SEG7_MAX_MSG_SIZE) {
        u8_bufflen = __SEG7_MAX_MSG_SIZE;
    }

    // Message is larger than our number of digits, so we need to scroll it (marquee style)
    if (u8_bufflen > __SEG7_NUM_DIGITS) {
        u8_newMode = SEG7_MODE_SCROLL;
    // Can fit the message without scrolling
    } else {
        u8_newMode = SEG7_MODE_STATIC;
    }

    // Don't do anything if we were previously in SCROLL mode and the message hasn't changed.
    // This will allow simpler mode logic in user apps.
    if ((u8_oldMode == SEG7_MODE_SCROLL) && !__seg7_hasScrollBuffChanged(pu8_data, u8_bufflen)) {
        return;
    }

    // Prepare for scroll
    if (u8_newMode == SEG7_MODE_SCROLL) {

        seg7_clearScreen();
        // Put chars in our scroll buffer
        for (u8_index = 0; u8_index < u8_bufflen; u8_index++) {
            __sz_seg7_msgScroll[u8_index] = pu8_data[u8_index];
        }
        // Add null term
        __sz_seg7_msgScroll[u8_bufflen] = '\0';
        // Setup scroll vars
        __u8_seg7_msgScrollLen = u8_bufflen;
        __u8_seg7_scrollIndex = 0;
        __u32_seg7_lastScrollTime = Hal_ReadTime32();

    // Prepare for static display
    } else if (u8_newMode == SEG7_MODE_STATIC) {

        seg7_clearScreen();
        // Put chars in our static display buffer
        for (u8_index = 0; u8_index < u8_bufflen; u8_index++) {
            __sz_seg7_currDisp[u8_index] = pu8_data[u8_index];
        }

    }

    // Set the new mode
    __u8_seg7_mode = u8_newMode;

    // Set display
    __seg7_update_displays();
}

void seg7_writeString(char* psz_data) {
    seg7_writeBuffer((uint8_t*)psz_data, strlen(psz_data));
}

void seg7_writeStringPadSpace(char* psz_data) {
    // New string on the stack to hold passed in string + the space padding + null term
    char newStr[strlen(psz_data) + strlen((char*)__sz_seg7_spaces) + 1];
    // Copy
    strcpy(newStr, psz_data);
    // Append Spaces
    strcat(newStr, (char*)__sz_seg7_spaces);
    // Call the normal string write function
    seg7_writeString(newStr);
}

char* seg7_getDisplay(void) {

    // Get static display buffer
    if (__u8_seg7_mode == SEG7_MODE_STATIC) {
        return (char*)__sz_seg7_currDisp;

    // Get message string
    } else if (__u8_seg7_mode == SEG7_MODE_SCROLL) {
        return (char*)__sz_seg7_msgScroll;

    } else {
        return 0;
    }
}

void seg7_blink(bool b_state) {

    bool b_lastState = __b_seg7_blinkOn;
    __b_seg7_blinkOn = b_state;

    // Reset the blinking timer when we first enable blinking
    if (!b_lastState && b_state) {
        __u32_seg7_lastBlinkTime = Hal_ReadTime32();
    }
}

void seg7_writeU8Decimal(uint8_t u8_data) {
    // Local Vars
    char sz_formBuff[5];

    __seg7_i32ToDecStr((int32_t)u8_data, sz_formBuff);

    __seg7_writeStringRightJust(sz_formBuff);
}

void seg7_writeU8Hex(uint8_t u8_data) {
    // Local Vars
    char sz_formBuff[5];

    __seg7_u8ToHexStr(u8_data, sz_formBuff);

    __seg7_writeStringRightJust(sz_formBuff);
}

void seg7_writeI8Decimal(int8_t i8_data) {
    // Local Vars
    char sz_formBuff[5];

    seg7_clearScreen();
    __seg7_i32ToDecStr((int32_t)i8_data, sz_formBuff);

    __seg7_writeStringRightJust(sz_formBuff);
}

void seg7_writeI8Hex(int8_t i8_data) {
    // We will not display "negative" hex numbers.
    seg7_writeU8Hex((uint8_t)i8_data);
}

void seg7_writeU16Decimal(uint16_t u16_data) {
    // Local Vars
    char sz_formBuff[10];

    __seg7_i32ToDecStr((int32_t)u16_data, sz_formBuff);

    // The number is too large to show on our display
    if (strlen(sz_formBuff) > __SEG7_NUM_DIGITS) {
        strcpy(sz_formBuff, (char*)__sz_seg7_dashes); // "overflow"
    }

    __seg7_writeStringRightJust(sz_formBuff);
}

void seg7_writeU16Hex(uint16_t u16_data) {
    // Local Vars
    char sz_formBuff[5];

    __seg7_u16ToHexStr(u16_data, sz_formBuff);

    __seg7_writeStringRightJust(sz_formBuff);
}

//***************************************************
// String conversion & utility functions
//***************************************************

// Convert ASCII char to 7 segment pattern using font table
uint8_t __seg7_char_to_patt(char c_char) {
    if ((c_char >= ' ') && (c_char <= '~')) {
        return (__7segfontTable[c_char - ' ']);
    } else {
        return 0;
    }
}

void __seg7_i32ToDecStr(int32_t i32_value, char* psz_outputStr) {
    char const csz_digit[] = "0123456789";

    // Add negative sign if needed
    if(i32_value < 0){
        *psz_outputStr++ = '-';
        i32_value *= -1;
    }

    int32_t i32_shiftVal = i32_value;
    // Move to where representation ends
    do { 
        ++psz_outputStr;
        i32_shiftVal = i32_shiftVal / 10;
    } while(i32_shiftVal);
    *psz_outputStr = '\0';

    // Move back, inserting digits as you go
    do { 
        *--psz_outputStr = csz_digit[i32_value % 10];
        i32_value = i32_value / 10;
    } while (i32_value);
}

/*  Print lower (num_bits) bits in binary format to psz_outputStr 
    Ex. 0x1234, 8 bits -> "0b00110100"
    psz_outputStr must be at least size (num_bits + 3), to accommodate prefix and null term
*/ 
void __seg7_u32ToBinStr(uint32_t u32_value, uint8_t num_bits, char* psz_outputStr) {

    if (num_bits == 0 || num_bits > 32)
    {
        return;
    }
    
    strcpy(psz_outputStr, "0b");    // Prefix

    // Convert bits to ASCII
    for (int i = 0; i < num_bits; i++)
    {
        // Check bits from left to right
        if (BIT_TST(u32_value, num_bits - 1 - i))
        {
            strcat(psz_outputStr, "1");
        } else {
            strcat(psz_outputStr, "0");
        }
        
    }
}

void __seg7_i32ToDecStrCat(int32_t i32_value, char* psz_outputStr) {
    char tmpstr[16];
    __seg7_i32ToDecStr(i32_value, tmpstr);
    strcat(psz_outputStr, tmpstr);
}

void __seg7_u32ToBinStrCat(uint32_t u32_value, uint8_t num_bits, char* psz_outputStr) {
    char tmpstr[35];    // 32 bits + 3
    __seg7_u32ToBinStr(u32_value, num_bits, tmpstr);
    strcat(psz_outputStr, tmpstr);
}

char __seg7_nibbleToHexChar(uint8_t u8_value) {
    // Check if the value is a regular digit
    if (u8_value <= 9) {
        // Return number char
        return '0' + u8_value;
    } else {
        // The value must be A-F, so send the correct char back; 10 = A, 15 = F
        return 'A' + (u8_value - 10);
    }
}

void __seg7_u8ToHexStr(uint8_t u8_value, char* psz_outputStr) {
    /*
     * Since we know we want to display 2 bytes every time, we mask out each 
     * individual nibble, then shift it to get the decimal value of that byte.
     * We then convert that value to a hex character and set it in the string
     */
    psz_outputStr[0] = __seg7_nibbleToHexChar(((u8_value & 0x00F0) >> 4));
    psz_outputStr[1] = __seg7_nibbleToHexChar(((u8_value & 0x000F) >> 0));
    psz_outputStr[2] = '\0';
}

void __seg7_u16ToHexStr(uint16_t u16_value, char* psz_outputStr) {
    /*
     * Since we know we want to display 4 bytes every time, we mask out each 
     * individual nibble, then shift it to get the decimal value of that byte.
     * We then convert that value to a hex character and set it in the string
     */
    psz_outputStr[0] = __seg7_nibbleToHexChar(((u16_value & 0xF000) >> 12));   
    psz_outputStr[1] = __seg7_nibbleToHexChar(((u16_value & 0x0F00) >> 8 ));
    psz_outputStr[2] = __seg7_nibbleToHexChar(((u16_value & 0x00F0) >> 4 ));
    psz_outputStr[3] = __seg7_nibbleToHexChar(((u16_value & 0x000F) >> 0 ));
    psz_outputStr[4] = '\0';
}

// Shift inChar in from the right, discard on the left
void __seg7_shiftLeftStr(char* psz_inStr, char c_inChar) {
    uint16_t u16_strLen = strlen(psz_inStr);

    // Index 0 to (len-2 = size-3), the next to last char in the string
    for (uint16_t u16_index = 0; u16_index <= u16_strLen-2; u16_index++) {
        psz_inStr[u16_index] = psz_inStr[u16_index + 1]; // Copy from right neighbor
    }

    // Insert the new char at the end
    psz_inStr[u16_strLen-1] = c_inChar;
}
