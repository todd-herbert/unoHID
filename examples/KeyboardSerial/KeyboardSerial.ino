/*
    Keyboard test

    For the Arduino UNO R3, and other ATmega328 based boards.

    Reads a byte from the serial port, sends a keystroke back.
    The sent keystroke is one higher than what's received, e.g. if you send a,
    you get b, send A you get B, and so forth.

    The circuit:
        - VUSB circuit, connected to D2, D4 and D5
            See https://github.com/todd-herbert/unoHID#wiring

    Created 21 Oct 2011
    Modified 27 Mar 2012
    by Tom Igoe

    Modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.

    https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardSerial
*/

#include "unoHID.h"

void setup() {
    // Open the serial port:
    Serial.begin(9600);

    // Initialize control over the keyboard:
    Keyboard.begin();
}

void loop() {

    // Check for incoming serial data
    if (Serial.available() > 0) {

        // Read incoming serial data:
        char inChar = Serial.read();

        // Type the next ASCII value from what you received:
        Keyboard.write(inChar + 1);
    }
    
}
