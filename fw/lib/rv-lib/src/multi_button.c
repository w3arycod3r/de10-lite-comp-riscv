// File:    multi_button.c
// Purpose:	Class functions

#include "multi_button.h"
#include "Hal.h"

/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold Test Sketch

http://jmsarduino.blogspot.com/2009/10/4-way-button-click-double-click-hold.html

By Jeff Saltzman
Oct. 13, 2009

To keep a physical interface as simple as possible, this sketch demonstrates generating
four output events from a single push-button.

1) Click:  rapid press and release
2) Double-Click:  two clicks in quick succession
3) Press and Hold:  holding the button down
4) Long Press and Hold:  holding the button for a long time

*/

/* btn_read should be active HIGH */
MB_EVENT_T mb_get_event(MULTIBUTTON_T* mb, bool btn_read)
{
    MB_EVENT_T event = MB_EVENT_NONE;
    mb->buttonVal = btn_read;
    uint32_t currTime = Hal_ReadTime32();

    // Button pressed down
    if (mb->buttonVal == true && mb->buttonLast == false && HAL_HAS_DURATION_PASSED_MSEC(mb->upTime, MB_DEBOUNCE_MSEC)) {

        mb->downTime = currTime;
        mb->ignoreUp = false;
        mb->waitForUp = false;
        mb->singleOK = true;
        mb->holdEventPast = false;
        mb->longHoldEventPast = false;
        if (!HAL_HAS_DURATION_PASSED_MSEC(mb->upTime, MB_DC_GAP_MSEC) && mb->DConUp == false && mb->DCwaiting == true)  mb->DConUp = true;
        else  mb->DConUp = false;
        mb->DCwaiting = false;

    // Button released
    } else if (mb->buttonVal == false && mb->buttonLast == true && HAL_HAS_DURATION_PASSED(mb->downTime, MB_DEBOUNCE_MSEC)) {       
        
        if (!mb->ignoreUp) {

            mb->upTime = currTime;
            if (mb->DConUp == false) {
                mb->DCwaiting = true;
            } else {
                event = MB_EVENT_DOUBLE_CLICK;
                mb->DConUp = false;
                mb->DCwaiting = false;
                mb->singleOK = false;
            }
        }
    }
    // Test for normal click event: DCgap expired
    if ( mb->buttonVal == false && HAL_HAS_DURATION_PASSED_MSEC(mb->upTime, MB_DC_GAP_MSEC) && mb->DCwaiting == true
         && mb->DConUp == false && mb->singleOK == true && event != MB_EVENT_DOUBLE_CLICK) {

        event = MB_EVENT_CLICK;
        mb->DCwaiting = false;
    }
    // Test for hold
    if (mb->buttonVal == true && HAL_HAS_DURATION_PASSED_MSEC(mb->downTime, MB_HOLD_TIME_MSEC)) {

        // Trigger "normal" hold
        if (!mb->holdEventPast) {

            event = MB_EVENT_HOLD;
            mb->waitForUp = true;
            mb->ignoreUp = true;
            mb->DConUp = false;
            mb->DCwaiting = false;
            //u32_downTime = t;         // Orig author commented this out..
            mb->holdEventPast = true;
        }
        // Trigger "long" hold
        if (HAL_HAS_DURATION_PASSED_MSEC(mb->downTime, MB_LONG_HOLD_TIME_MSEC)) {

            if (!mb->longHoldEventPast) {
                
                event = MB_EVENT_LONG_HOLD;
                mb->longHoldEventPast = true;
            }
        }
    }

    mb->buttonLast = mb->buttonVal;
    return event;
}

void mb_init(MULTIBUTTON_T* mb)
{
    mb_reset(mb);
}

void mb_reset(MULTIBUTTON_T* mb)
{
    mb->buttonVal = false;
    mb->buttonLast = false;
    mb->DCwaiting = false;
    mb->DConUp = false;
    mb->singleOK = true;
    mb->downTime = 0;
    mb->upTime = 0;
    mb->ignoreUp = false;
    mb->waitForUp = false;
    mb->holdEventPast = false;
    mb->longHoldEventPast = false;
}