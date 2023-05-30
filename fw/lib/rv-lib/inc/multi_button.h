// File:    multi_button.h
// Purpose:	Class definition

#ifndef MULTIBUTTON_H
#define MULTIBUTTON_H

#include <stdbool.h>
#include <stdint.h>

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

// -------- DEFINES --------
#define MB_DEBOUNCE_MSEC        20      // (Def 20) ms debounce period to prevent flickering when pressing or releasing the button
#define MB_DC_GAP_MSEC          250     // (Def 250) (Set to 0 to disable DC) max ms between clicks for a double click event
#define MB_HOLD_TIME_MSEC       1000    // (Def 1000) ms hold period: how long to wait for press+hold event
#define MB_LONG_HOLD_TIME_MSEC  3000    // (Def 3000) ms long hold period: how long to wait for press+hold event

typedef enum {
    MB_EVENT_NONE,
    MB_EVENT_CLICK,
    MB_EVENT_DOUBLE_CLICK,
    MB_EVENT_HOLD,
    MB_EVENT_LONG_HOLD,
} MB_EVENT_T;


typedef struct {

    bool buttonVal;         // value read from button, active HIGH
    bool buttonLast;        // buffered value of the button's previous state, active HIGH
    bool DCwaiting;         // whether we're waiting for a double click (down)
    bool DConUp;            // whether to register a double click on next release, or whether to wait and click
    bool singleOK;          // whether it's OK to do a single click
    uint32_t downTime;      // time the button was pressed down
    uint32_t upTime;        // time the button was released
    bool ignoreUp;          // whether to ignore the button release because the click+hold was triggered
    bool waitForUp;         // when held, whether to wait for the up event
    bool holdEventPast;     // whether or not the hold event happened already
    bool longHoldEventPast; // whether or not the long hold event happened already

} MULTIBUTTON_T;

void mb_init(MULTIBUTTON_T* mb);
void mb_reset(MULTIBUTTON_T* mb);
MB_EVENT_T mb_get_event(MULTIBUTTON_T* mb, bool btn_read);

#endif