/*
    Keyboard Message test

    For the Arduino UNO R3, and other ATmega328 based boards.

    Sends a text string when a button is pressed.

    The circuit:
        - VUSB circuit, connected to D2, D4 and D5
                See https://github.com/todd-herbert/unoHID#wiring

        - Push-button attached from pin 8 to +5V

        - 10 kilo-ohm resistor attached from pin 8 to ground

    Created 24 Oct 2011
    Modified 27 Mar 2012
    by Tom Igoe
    Modified 11 Nov 2013
    by Scott Fitzgerald

    Modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.

    https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardMessage
*/

#include "unoHID.h"

const int buttonPin = 8;            // Input pin for push-button
int previousButtonState = HIGH;     // For checking the state of a push-button
int counter = 0;                    // Button push counter


void setup() {
    pinMode(buttonPin, INPUT);      // Make the push-button pin an input
    
    Keyboard.begin();               // Initialize control over the keyboard
}


void loop() {
    // Read the push-button:
    int buttonState = digitalRead(buttonPin);   

    // If the button state has changed, and it is currently pressed
    if ((buttonState != previousButtonState) && (buttonState == HIGH)) {
        // Increment the button counter
        counter++;

        // Type out a message
        Keyboard.print("You pressed the button ");
        Keyboard.print(counter);
        Keyboard.println(" times.");
    }

    // Save the current button state for comparison next time:
    previousButtonState = buttonState;
}