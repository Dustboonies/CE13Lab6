// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
#include "Buttons.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
uint8_t but = 0;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    uint8_t counter = LATE;
    LEDS_INIT;
    ButtonsInit();
    while (1) {
        but = ButtonsCheckEvents();
        //event check
        if (but != 0) {
            counter = LATE;
            if (SW1 == 1 && (but & BUTTON_EVENT_1UP) == BUTTON_EVENT_1UP) {
                if ((counter & 0x03) == 0x03) {
                    counter = counter & 0xFC;
                } else {
                    counter += 0x03;
                }
                
            } else if (SW1 == 0 && (but & BUTTON_EVENT_1DOWN) == BUTTON_EVENT_1DOWN) {
                if ((counter & 0x03) == 0x03) {
                    counter = counter & 0xFC;
                } else {
                    counter += 0x03;
                }
            }
            if (SW2 == 1 && (but & BUTTON_EVENT_2UP) == BUTTON_EVENT_2UP) {
                if ((counter & 0x0C) == 0x0C) {
                    counter = counter & 0xF3;
                } else {
                    counter += 0x0C;
                }

            } else if (SW2 == 0 && (but & BUTTON_EVENT_2DOWN) == BUTTON_EVENT_2DOWN) {
                if ((counter & 0x0C) == 0x0C) {
                    counter = counter & 0xF3;
                } else {
                    counter += 0x0C;
                }
            }
            if (SW3 == 1 && (but & BUTTON_EVENT_3UP) == BUTTON_EVENT_3UP) {
                if ((counter & 0x30) == 0x30) {
                    counter = counter & 0xCF;
                } else {
                    counter += 0x30;
                }

            } else if (SW3 == 0 && (but & BUTTON_EVENT_3DOWN) == BUTTON_EVENT_3DOWN) {
                if ((counter & 0x30) == 030) {
                    counter = counter & 0xCF;
                } else {
                    counter += 0x30;
                }
            }
            if (SW4 == 1 && (but & BUTTON_EVENT_4UP) == BUTTON_EVENT_4UP) {
                if ((counter & 0xC0) == 0xC0) {
                    counter = counter & 0x3F;
                } else {
                    counter += 0xC0;
                }

            } else if (SW4 == 0 && (but & BUTTON_EVENT_4DOWN) == BUTTON_EVENT_4DOWN) {
                if ((counter & 0xC0) == 0xC0) {
                    counter = counter & 0x3F;
                } else {
                    counter += 0xC0;
                }
            }
            LEDS_SET(counter);
            but = 0;

        }
        //        if (SW1 == 1) {
        //            if ((but & 0x01) == 0x01) {
        //                counter += 0x03;
        //            }
        //        } else if (SW1 == 0){
        //            if ((but&0x01) == 0x00){
        //                counter += 0x03;
        //            }
        //        }
        //        if (SW2 == 1) {
        //            if ((but & 0x02) == 0x02) {
        //                counter += 0x0C;
        //            }
        //        }else if (SW2 == 0){
        //            if ((but&0x02) == 0x00){
        //                counter += 0x0C;
        //            }
        //        }
        //        if (SW3 == 1) {
        //            if ((but & 0x04) == 0x04) {
        //                counter += 0x30;
        //            }
        //        }else if (SW3 == 0){
        //            if ((but&0x04) == 0x00){
        //                counter += 0x30;
        //            }
        //        }
        //        if (SW4 == 1) {
        //            if ((but & 0x08) == 0x08) {
        //                counter += 0xC0;
        //            }
        //        }else if (SW4 == 0){
        //            if ((but&0x08) == 0x00){
        //                counter += 0xC0;
        //            }
        //        }
        //        LEDS_SET(counter);

    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    ButtonsCheckEvents();

}