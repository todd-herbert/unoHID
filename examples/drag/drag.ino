#define PROFILE_WINDOWS
//#define PROFILE_WINXP
//#define PROFILE_ANDROID
//#define PROFILE_MAC
//#define PROFILE_UBUNTU

#include <absolute-mouse.h>

void setup() {
    AbsoluteMouse.begin();

    // Click and drag from near top left, to near bottom right, taking 2000ms
    AbsoluteMouse.drag(25, 25, -25, -25, 2000);
}

void loop() {
    
}