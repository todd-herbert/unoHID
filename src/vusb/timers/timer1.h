//This file is included conditionally by the preprocessor, if we should be polling using timer 1

#include <Arduino.h>
#include "vusb/driver/usbdrv.h"

#ifdef POLL_WITH_TIMER1

    ISR(TIMER1_COMPA_vect){
        VUSB.poll();   //Every 8ms - 125Hz
    }

#endif

