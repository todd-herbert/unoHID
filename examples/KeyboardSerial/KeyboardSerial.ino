/*
    Keyboard test

    For the Arduino UNO R3, and other ATmega328 based boards.

    Reads a byte from the serial port, sends a keystroke back.
    The sent keystroke is one higher than what's received, e.g. if you send a,
    you get b, send A you get B, and so forth.

    The circuit:
        - VUSB circuit, connected to D2, D4 and D5
            TODO: link to vusb wiring

    created 21 Oct 2011
    modified 27 Mar 2012
    by Tom Igoe

    modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.

    https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardSerial
*/

#include "unoHID.h"

void setup() {
    // open the serial port:
    Serial.begin(9600);
    // initialize control over the keyboard:
    Keyboard.begin();
}

void loop() {
    // check for incoming serial data:
    if (Serial.available() > 0) {
        // read incoming serial data:
        char inChar = Serial.read();
        // Type the next ASCII value from what you received:
        Keyboard.write(inChar + 1);
    }
}
