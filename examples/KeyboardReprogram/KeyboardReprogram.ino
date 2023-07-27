/*

    Arduino Programs Blink

    For the Arduino UNO R3, and other ATmega328 based boards.

    When you connect pin 2 to ground, it creates a new window with a key
    combination (CTRL-N), then types in the Blink sketch, then auto-formats the
    text using another key combination (CTRL-T), then uploads the sketch to the
    currently selected Arduino using a final key combination (CTRL-U).

    NOTE: this sketch only works when run in Arduino IDE 2

    Circuit:

        - VUSB circuit, connected to D2, D4 and D5
            TODO: link to vusb wiring

        - wire to connect D2 to ground

    created 5 Mar 2012
    modified 29 Mar 2012
    by Tom Igoe
    modified 3 May 2014
    by Scott Fitzgerald

    modified 20 Jul 2023
    for unoHID library

    This example is in the public domain.

    https://www.arduino.cc/en/Tutorial/KeyboardReprogram

*/

#include "unoHID.h"

// use this option for Windows and Linux.
// leave commented out if using OSX:
 char ctrlKey = KEY_LEFT_CTRL;

// use this option for OSX.
// Comment it out if using Windows or Linux:
// char ctrlKey = KEY_LEFT_GUI;

void setup() {

    // make pin 8 an input and turn on the pull-up resistor 
    // so it goes high unless connected to ground:

    pinMode(8, INPUT_PULLUP);

    // initialize control over the keyboard:

    Keyboard.begin();
}

void loop() {

    while (digitalRead(8) == HIGH) {
        // do nothing until pin 8 goes low
        delay(500);
    }

    delay(1000);

    // new document:

    Keyboard.press(ctrlKey);
    Keyboard.press('n');

    delay(100);
    Keyboard.releaseAll();

    // wait for new window to open:
    // (slow..)

    delay(10000);

    // versions of the Arduino IDE after 1.5 pre-populate new sketches with
    // setup() and loop() functions let's clear the window before typing anything new

    // select all

    Keyboard.press(ctrlKey);
    Keyboard.press('a');

    delay(500);
    Keyboard.releaseAll();

    // delete the selected text

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

    // make it 1000 instead:

    Keyboard.println("1000);");
    Keyboard.println("digitalWrite(13, LOW);");
    Keyboard.println("delay(1000);");
    Keyboard.println("}");

    // tidy up (autoformat):
    Keyboard.press(ctrlKey);
    Keyboard.press('t');

    delay(100);
    Keyboard.releaseAll();

    delay(3000);

    // upload code:

    Keyboard.press(ctrlKey);
    Keyboard.press('u');

    delay(100);
    Keyboard.releaseAll();

    // wait for the sweet oblivion of reprogramming:
    while (true);
}