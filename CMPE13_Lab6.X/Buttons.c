
#include <stdint.h>

// We rely on this file to define various macros for working with the hardware buttons.
#include "BOARD.h"
#include "Buttons.h"

//Variable declarations
uint8_t fourth = 0, third = 0, second = 0, first = 0, current = 0, increment = 0;
ButtonEventFlags bft;

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

/**
 * This function checks the button states and returns any events that have occured since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * This function also performs debouncing of the buttons. Every time ButtonsCheckEvents() is called,
 * all button values are saved, up to the 4th sample in the past, so 4 past samples and the present
 * values. A button event is triggered if the newest 4 samples are the same and different from the
 * oldest sample. For example, if button 1 was originally down, button 1 must appear to be up for 4
 * samples before a BUTTON_EVENT_1UP is triggered. This eliminates button bounce, where the button
 * may quickly oscillate between the ON and OFF state as it's being pressed or released.
 *
 * NOTE: This will not work properly without ButtonsInit() being called beforehand.
 * @return A bitwise-ORing of the constants in the ButtonEventFlags enum or BUTTON_EVENT_NONE if no
 *         event has occurred.
 */
uint8_t ButtonsCheckEvents(void)
{
    increment = 0;
    current = BUTTON_STATES();
    if (((current == first) == (second == third)) != fourth) {
        fourth = third;
        third = second;
        second = first;
        first = current;
        if ((current & 0x01) == 0x01) {
            increment += 0x01;
        } else {
            increment += 0x02;
        }
        if ((current & 0x02) == 0x02) {
            increment += 0x04;
        } else {
            increment += 0x08;
        }
        if ((current & 0x04) == 0x04) {
            increment += 0x10;
        } else {
            increment += 0x20;
        }
        if ((current & 0x08) == 0x08) {
            increment += 0x40;
        } else {
            increment += 0x80;
        }
        return increment;
    }
    fourth = third;
    third = second;
    second = first;
    first = current;
    return 0;
}


