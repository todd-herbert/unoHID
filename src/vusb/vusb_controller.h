#ifndef __VUSB_CONTROLLER_H__
#define __VUSB_CONTROLLER_H__

#include <Arduino.h>
#include "vusb/driver/usbdrv.h"

class VUSBController {
    public:
        // Store the timer which was selected with macros in unoHID.h
        enum PollingTimer : int8_t { Timer2 = 2, Timer1 = 1, Manual = -1 }; 

        // Insist on a timer
        VUSBController() = delete;
        VUSBController(PollingTimer timer);

        void mouseOff();
        void mouseOn();
        void keyboardOff();
        void keyboardOn();

        void poll();
        void pausePolling();
        void resumePolling();

    private:
        void begin();
        void end();

    // Members
    private:
        bool mouseEnabled = false;
        bool keyboardEnabled = false;
        
        PollingTimer polling_timer;
        volatile bool autopolling_paused = false;
} ;

#endif
