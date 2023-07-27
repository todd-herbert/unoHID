/*
 * This file describes a digitizer / graphics tablet device, along with supporting methods.
 * The included VUSB library will perform the appropriate USB communications.
 */

#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <Arduino.h>
#include "vusb/vusb_controller.h"
#include "vusb/driver/usbdrv.h"

enum MouseButton : uint8_t {MOUSE_LEFT = 1, MOUSE_RIGHT = 2, MOUSE_MIDDLE = 3};

class MouseDevice {
    //Methods
    public:
        MouseDevice() = delete;
        MouseDevice( VUSBController *vusb );
        void begin();
        void end();

        void move(int16_t x, int16_t y, int8_t wheel = 0);
        void press(MouseButton button = MOUSE_LEFT);
        void release(MouseButton button = MOUSE_LEFT);
        void click(MouseButton button = MOUSE_LEFT);
        
        bool isPressed(MouseButton button = MOUSE_LEFT);

        // Extra Methods

        void doubleClick(MouseButton button = MOUSE_LEFT);
        void longClick(uint16_t duration, MouseButton button = MOUSE_LEFT);
        void scroll(int16_t amount);

        void setTxDelay(uint16_t delay);
        void update();                                              // Send Mouse HID report

    private:

        void setButton(MouseButton button, bool state);             // Configure byte 1 of report

        VUSBController *vusb_controller;

    private:
        uint16_t tx_delay = 0;
        uint8_t report[7] = {0x01, 0, 0, 0, 0, 0, 0};  //Bit 0 is ReportID 1, to show that we're sending mouse data
} ;

#endif //__MOUSE_H__