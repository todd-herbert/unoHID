#define PROFILE_WINDOWS
//#define PROFILE_WINXP
//#define PROFILE_ANDROID
//#define PROFILE_MAC
//#define PROFILE_UBUNTU

#include <absolute-mouse.h>

void setup() {
    AbsoluteMouse.begin();

    AbsoluteMouse.position(50,50);  // Move the cursor to screen center, but perform no action
    delay(1000);

    AbsoluteMouse.position(50, 30); // Move the cursor to horizontal center, 30% from top
    delay(1000);

    // With EMULATE_MOUSE, several commands do not require explicit x and y arguments.
    // The command will be performed wherever the cursor is currently placed
    AbsoluteMouse.click();

    // Move the cursor to screen center, and scroll up by "1"
    // The distance scrolled depends on your device's mouse settings, so some trial and error is needed to find the correct value
    AbsoluteMouse.scroll(50, 50, -1);
    delay(1000);

    AbsoluteMouse.scroll(1);    //Scroll back down
}

void loop() {
    
}