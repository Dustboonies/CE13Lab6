// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
#include "Oled.h"
// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

typedef struct ADCTimerData {
    int event;
    uint16_t value;
} ADCTimerData;
// **** Define global, module-level, or external variables here ****
ADCTimerData adt;
// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Enable interrupts for the ADC
    ConfigIntADC10(ADC_INT_PRI_2 | ADC_INT_SUB_PRI_0 | ADC_INT_ON);

    // Set B2 to an input so AN0 can be used by the ADC.
    TRISBCLR = 1 << 2;

    // Configure and start the ADC
    // Read AN0 as sample a. We don't use alternate sampling, so setting sampleb is pointless.
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2);
    OpenADC10(
            ADC_MODULE_ON | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG16 | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON,
            ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_8 |
            ADC_BUF_16 | ADC_ALT_INPUT_OFF,
            ADC_SAMPLE_TIME_29 | ADC_CONV_CLK_PB | ADC_CONV_CLK_51Tcy2,
            ENABLE_AN2_ANA,
            SKIP_SCAN_ALL
            );
    EnableADC10();

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    OledInit();
    adt.event = 0;
    adt.value = 0;
    int perc = 0;
    char str[100];
    while (1) {

        if (adt.event == 1) {
            perc = (int) adt.value * 100 / 1023;
            sprintf(str, "%d   %d %%     ", adt.value, perc);
            OledDrawString(str);
            OledUpdate();
        }
        adt.event = 0;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the ISR for the ADC1 peripheral. It runs continuously triggered by the hardware directly
 * and not called by any code, as all interrupts are. During each run it reads all 8 samples from
 * the ADC (registers ADC1BUF0 through ADC1BUF7), averages them, and stores them in a module-level
 * AdcResult struct. If this averaged ADC value has changed between this run and the last one, the
 * event flag in the AdcResult struct is set to true.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void)
{
    //Clear the interrupt flag.
    INTClearFlag(INT_AD1);
    double avg = (double) (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 + ADC1BUF8) / 8;
    if (adt.value != avg) {
        adt.event = 1;
    }
    adt.value = avg;

}