//This file is included conditionally by the preprocessor, if we should be polling using timer 1

#include <Arduino.h>
#include "vusb/usbdrv.h"

ISR(TIMER1_COMPA_vect){
   usbPoll();   //Every 50 ms
}

class PollingSetupHelper {
    public:
        PollingSetupHelper() {
            // TIMER 1 for interrupt frequency 20 Hz:
            cli(); // stop interrupts
            TCCR1A = 0; // set entire TCCR1A register to 0
            TCCR1B = 0; // same for TCCR1B
            TCNT1  = 0; // initialize counter value to 0
            // set compare match register for 20 Hz increments
            OCR1A = 12499; // = 16000000 / (64 * 20) - 1 (must be <65536)
            // turn on CTC mode
            TCCR1B |= (1 << WGM12);
            // Set CS12, CS11 and CS10 bits for 64 prescaler
            TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
            // enable timer compare interrupt
            TIMSK1 |= (1 << OCIE1A);
            sei(); // allow interrupts
        }
} __POLLING_WITH_TIMER_1__;

