#define PROFILE_WINDOWS
//#define PROFILE_WINXP
//#define PROFILE_ANDROID
//#define PROFILE_MAC
//#define PROFILE_UBUNTU

#include <absolute-mouse.h>

void setup() {
    AbsoluteMouse.begin();
    AbsoluteMouse.click(50, 50);    // Click middle of screen
}

void loop() { 
        
}