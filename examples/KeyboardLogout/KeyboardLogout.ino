/*
    Keyboard logout

    This sketch demonstrates the Keyboard library.
    When you connect pin 8 to ground, it performs a logout.

    It uses keyboard combinations to do this, as follows:

        - On Windows, CTRL-ALT-DEL followed by ALT-S

        - On Ubuntu, CTRL-ALT-DEL, right arrow, and ENTER

        - On OSX, CMD-SHIFT-Q

    Circuit:

        - Arduino UNO R3, or other ATmega328 based board, wired for V-USB
            TODO: link to vusb wiring

        - Wire to connect D8 to ground (or button)

    Created 6 Mar 2012
    Modified 27 Mar 2012
    by Tom Igoe

    Modified 20 Jul 2023
    for unoHID library

    This example is in the public domain.

    https://www.arduino.cc/en/Tutorial/KeyboardLogout

*/

#define OSX 0
#define WINDOWS 1
#define UBUNTU 2

#include "unoHID.h"

// Change this to match your platform:
int platform = WINDOWS;

void setup() {

    // Make pin 8 an input and turn on the pull-up resistor 
    // so it goes high unless connected to ground:
    pinMode(8, INPUT_PULLUP);

    Keyboard.begin();
}

void loop() {

    while (digitalRead(8) == HIGH) {
        // Do nothing until pin 2 goes low
        delay(100);
    }

    delay(1000);

    switch (platform) {
        case OSX:
            Keyboard.press(KEY_LEFT_GUI);

            // Shift-Q logs out:
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press('Q');
            delay(100);
            Keyboard.releaseAll();

            // enter:
            Keyboard.write(KEY_RETURN);
            break;

        case WINDOWS:

            // CTRL-ALT-DEL:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_DELETE);
            delay(100);
            Keyboard.releaseAll();

            // ALT-S:
            delay(2000);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press('s');
            Keyboard.releaseAll();

            break;

        case UBUNTU:

            // CTRL-ALT-DEL:
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_ALT);
            Keyboard.press(KEY_DELETE);
            delay(1000);
            Keyboard.releaseAll();

            // Right
            Keyboard.write(KEY_RIGHT_ARROW);

            // Enter to confirm logout:
            Keyboard.write(KEY_RETURN);

        break;
    }

    // Do nothing:
    while (true);
}