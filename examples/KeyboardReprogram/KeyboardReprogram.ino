/*
    Arduino Programs Blink

    For the Arduino UNO R3, and other ATmega328 based boards.

    When you connect pin 8 to ground, it creates a new window with a key
    combination (CTRL-N), then types in the Blink sketch, then auto-formats the
    text using another key combination (CTRL-T), then uploads the sketch to the
    currently selected Arduino using a final key combination (CTRL-U).

    NOTE: this sketch only works when run in Arduino IDE 2

    Circuit:

        - VUSB circuit, connected to D2, D4 and D5
            See https://github.com/todd-herbert/unoHID#wiring

        - Wire to connect D8 to ground

    Created 5 Mar 2012
    Modified 29 Mar 2012
    by Tom Igoe
    Modified 3 May 2014
    by Scott Fitzgerald

    Modified 20 Jul 2023
    for unoHID library

    This example is in the public domain.

    https://www.arduino.cc/en/Tutorial/KeyboardReprogram

*/

#include "unoHID.h"

// Use this option for Windows and Linux.
// Comment out if using OSX:

char ctrlKey = KEY_LEFT_CTRL;

// Use this option for OSX.
// Comment it out if using Windows or Linux:

// char ctrlKey = KEY_LEFT_GUI;

void setup() {

    // Make pin 8 an input and turn on the pull-up resistor 
    // So it goes high unless connected to ground:

    pinMode(8, INPUT_PULLUP);

    // Initialize control over the keyboard:

    Keyboard.begin();
}

void loop() {

    while (digitalRead(8) == HIGH) {
        // Do nothing until pin 8 goes low
        delay(100);
    }

    delay(1000);

    // New document:

    Keyboard.press(ctrlKey);
    Keyboard.press('n');

    delay(100);
    Keyboard.releaseAll();

    // Wait for new window to open:
    // (slow..)

    delay(10000);

    // Versions of the Arduino IDE after 1.5 pre-populate new sketches with
    // setup() and loop() functions let's clear the window before typing anything new

    // Select all

    Keyboard.press(ctrlKey);
    Keyboard.press('a');

    delay(500);
    Keyboard.releaseAll();

    // Delete the selected text

    Keyboard.write(KEY_BACKSPACE);
    delay(500);

    // Type out "blink":

    Keyboard.print("void setup() {");
    delay(50);
    Keyboard.write(KEY_DELETE); // Get rid of autocomplete braces
    Keyboard.println();

    Keyboard.println("pinMode(13, OUTPUT);");
    Keyboard.println("}");
    Keyboard.println();

    Keyboard.print("void loop() {");
    delay(50);
    Keyboard.write(KEY_DELETE); // Get rid of autocomplete braces
    Keyboard.println();

    Keyboard.println("digitalWrite(13, HIGH);");
    Keyboard.print("delay(3000);");

    // 3000 ms is too long. Delete it:

    for (int keystrokes = 0; keystrokes < 6; keystrokes++) {
        delay(500);
        Keyboard.write(KEY_BACKSPACE);
    }

    // Make it 1000 instead:

    Keyboard.println("1000);");
    Keyboard.println("digitalWrite(13, LOW);");
    Keyboard.println("delay(1000);");
    Keyboard.println("}");

    // Tidy up (autoformat):
    Keyboard.press(ctrlKey);
    Keyboard.press('t');

    delay(100);
    Keyboard.releaseAll();

    delay(3000);

    // Upload code:

    Keyboard.press(ctrlKey);
    Keyboard.press('u');

    delay(100);
    Keyboard.releaseAll();

    // Wait for the sweet oblivion of reprogramming:
    while (true);
}