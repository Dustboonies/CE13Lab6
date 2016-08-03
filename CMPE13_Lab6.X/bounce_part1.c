// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

typedef struct TimerResult {
    uint8_t event;
    uint8_t value;
} TimerResult;
// **** Define global, module-level, or external variables here ****
TimerResult tr;
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

    LEDS_INIT;
    //  printf("%d", LEDS_GET);
    tr.event = 0;
    tr.value = 0;
    int direction = 0;
    uint8_t leeds = 0x01;
    while (1) {
        LEDS_SET(leeds);
        if (tr.event == 1) {
            if (leeds == 0x80) {
                direction = 1;
            } else if (leeds == 0x01) {
                direction = 0;
            }
            if (direction == 1) {
                leeds = leeds >> 1;
            } else if (direction == 0) {
                leeds = leeds << 1;
            }

        }
        tr.event = 0;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    tr.value++;

    INTClearFlag(INT_T1);
    if (tr.value > SWITCH_STATES()) {
        tr.event = 1;
        tr.value = 0;
    }
}