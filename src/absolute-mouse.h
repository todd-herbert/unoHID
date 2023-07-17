#ifndef __ABSOLUTE_MOUSE_H__
#define __ABSOLUTE_MOUSE_H__

#include <Arduino.h>
#include "vusb/hid_descriptor.h"
#include "vusb/usbFunctionSetup.h"  //Define, once only, a required extern function of vusb, which we don't actually use

    // Select from profiles
    #if defined(PROFILE_WINDOWS)
        #define EMULATE_MOUSE
    
    #elif defined(PROFILE_WINXP)
        #define EMULATE_MOUSE
        #if !defined(POLL_WITH_TIMER2)
            #define POLL_WITH_TIMER1
        #endif

    #elif defined(PROFILE_ANDROID) 
        #define EMULATE_STYLUS

    #elif defined(PROFILE_MAC)
        #define EMULATE_HYBRID
        #define ENABLE_HOMING

    #elif defined(PROFILE_UBUNTU)
        #define EMULATE_MOUSE
        #define ENABLE_HOMING
        #if !defined(POLL_WITH_TIMER2)
            #define POLL_WITH_TIMER1
        #endif

    #else 
    #endif


    // Pick a mode    
    #if defined(EMULATE_MOUSE)
        #include "mouse/mouse.h"
        static MouseDevice AbsoluteMouse;

    #elif defined(EMULATE_STYLUS)
        #include "stylus/stylus.h"
        static StylusDevice AbsoluteMouse;

    #elif defined(EMULATE_HYBRID)
        #include "hybrid/hybrid.h"
        static HybridDevice AbsoluteMouse;
    #else
        #error Please define a profile at the top of your program. See https://github.com/todd-herbert/absolute-mouse/blob/main/README.md#configuration
    #endif
    
    // Pick a timer to poll with
    #if defined(POLL_WITH_TIMER1)
        #include "timers/timer1.h"
    #elif defined(POLL_WITH_TIMER2)
        #include "timers/timer2.h"
    #endif

    //Enable or disable homing
    #if defined(ENABLE_HOMING)
        void conditional_homing() { AbsoluteMouse.perform_homing();}
    #else
        void conditional_homing() { /* Don't perform homing */ }
    #endif

#endif