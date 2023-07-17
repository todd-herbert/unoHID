//This file is included conditionally by the preprocessor, if we should be polling using timer 1

#include <Arduino.h>
#include "vusb/usbdrv.h"

class PollingSetupHelper {
    public:
        PollingSetupHelper() {
            // TIMER 2 for interrupt frequency 1000 Hz:
            cli(); // stop interrupts
            TCCR2A = 0; // set entire TCCR2A register to 0
            TCCR2B = 0; // same for TCCR2B
            TCNT2  = 0; // initialize counter value to 0
            // set compare match register for 1000 Hz increments
            OCR2A = 249; // = 16000000 / (64 * 1000) - 1 (must be <256)
            // turn on CTC mode
            TCCR2B |= (1 << WGM21);
            // Set CS22, CS21 and CS20 bits for 64 prescaler
            TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);
            // enable timer compare interrupt
            TIMSK2 |= (1 << OCIE2A);
            sei(); // allow interrupts
        }
} __POLLING_WITH_TIMER_2__;

volatile uint16_t __POLLING_WITH_TIMER_2_COUNTER__ = 0;

ISR(TIMER2_COMPA_vect){
    __POLLING_WITH_TIMER_2_COUNTER__++;
    if (__POLLING_WITH_TIMER_2_COUNTER__ == 50) {
        __POLLING_WITH_TIMER_2_COUNTER__ = 0;
        usbPoll();  //Every 50ms
    };
}

