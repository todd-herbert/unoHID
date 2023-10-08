#ifndef __UNOHID_H__
#define __UNOHID_H__

#include <Arduino.h>

// V-USB 
#include "vusb/usb_descriptor.h"        // Define the USB device
#include "vusb/usbFunctionSetup.h"      // Define a required extern function of vusb, which we don't actually use
#include "vusb/vusb_controller.h"       // Background class - communication between Mouse and Keyboard


// User-facing libraries
#include "mouse/mouse.h"
#include "keyboard/keyboard.h"

// If using a keepalive pin (bugfix for Arduino nano)
#ifdef PIN_KEEPALIVE
    #ifndef ARDUINO_AVR_NANO
        #pragma message "Note: Workaround enabled. PIN_KEEPALIVE should be wired to RST"
    #else
        #warning PIN_KEEPALIVE workaround may not be relevant for your Arduino model
    #endif
#else
    #define PIN_KEEPALIVE -1
#endif

// Config V-USB, with specified timer
#if defined(POLL_MANUALLY)
    #pragma message "Note: Manual polling selected. Remember to call VUSB.poll() in loop"
    VUSBController VUSB(VUSBController::PollingTimer::Manual, PIN_KEEPALIVE);

#elif defined(POLL_WITH_TIMER1)
    #pragma message "Note: Timer 1 is selected for polling."
    VUSBController VUSB(VUSBController::PollingTimer::Timer1, PIN_KEEPALIVE);
    #include "vusb/timers/timer1.h"

#else   // Timer 2, default
    VUSBController VUSB(VUSBController::PollingTimer::Timer2, PIN_KEEPALIVE);
    #include "vusb/timers/timer2.h"
#endif


// Instantiate the main classes
MouseDevice Mouse( &VUSB );
Keyboard_ Keyboard( &VUSB );

#endif