//This file is included conditionally by the preprocessor, if we should be polling using timer 1

#include <Arduino.h>
#include "vusb/driver/usbdrv.h"

// volatile uint16_t __POLLING_WITH_TIMER_2_COUNTER__ = 0;

// Timer set for 100Hz - 10ms
ISR(TIMER2_COMPA_vect){
    VUSB.poll();
}