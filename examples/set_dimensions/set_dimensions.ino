#define PROFILE_WINDOWS
//#define PROFILE_WINXP
//#define PROFILE_ANDROID
//#define PROFILE_MAC
//#define PROFILE_UBUNTU

#include <absolute-mouse.h>

void setup() {
    // You can set dimensions in begin()
    AbsoluteMouse.begin(100, 100);
    AbsoluteMouse.click(50, 50);    // Middle of screen

    AbsoluteMouse.set_dimensions(1440, 900);
    AbsoluteMouse.click(720, 450);  // This also clicks middle of screen

    AbsoluteMouse.set_dimensions(310, 175);     // Screen is 310mm wide x 175mm tall
    AbsoluteMouse.click(310 / 2,  175 / 2);     // This now clicks middle of screen

    AbsoluteMouse.click(155, 87);   // (Just to be clear, you do not need to call .set_dimensions() before each command)
}

void loop() {
    
}