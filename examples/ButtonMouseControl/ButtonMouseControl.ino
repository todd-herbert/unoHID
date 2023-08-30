/*
    ButtonMouseControl

    Controls the mouse from five push-buttons on an Arduino UNO R3, or other
    ATmega328 based board.

    Hardware:
        - VUSB circuit, connected to D2, D4 and D5
                TODO: link to vusb wiring

        - Five push-buttons attached to D8, D9, D10, D11, D12
                NOTE: The pins differ from those mentioned in the attached tutorial.

    The mouse movement is always relative. This sketch reads four push-buttons, and
    uses them to set the movement of the mouse.

    WARNING: When you use the Mouse.move() command, the Arduino takes over your
    mouse! Make sure you have control before you use the mouse commands.

    Created 15 Mar 2012
    Modified 27 Mar 2012
    by Tom Igoe

    modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.

    https://www.arduino.cc/en/Tutorial/ButtonMouseControl

*/

#include "unoHID.h"


// Set pin numbers for the five buttons:
const int upButton = 8;
const int downButton = 9;
const int leftButton = 10;
const int rightButton = 11;
const int mouseButton = 12;

int range = 5;              // Output range of X or Y movement; affects movement speed
int responseDelay = 10;     // Response delay of the mouse, in ms


void setup() {

    // Initialize the buttons' inputs:
    pinMode(upButton, INPUT);
    pinMode(downButton, INPUT);
    pinMode(leftButton, INPUT);
    pinMode(rightButton, INPUT);
    pinMode(mouseButton, INPUT);

    // initialize mouse control:
    Mouse.begin();

}

void loop() {

    // Read the buttons:
    int upState = digitalRead(upButton);
    int downState = digitalRead(downButton);
    int rightState = digitalRead(rightButton);
    int leftState = digitalRead(leftButton);
    int clickState = digitalRead(mouseButton);

    // Calculate the movement distance based on the button states:
    int xDistance = (rightState - leftState) * range;
    int yDistance = (downState- upState) * range;

    // If X or Y is non-zero, move:
    if ( (xDistance != 0) || (yDistance != 0) ) {
        Mouse.move(xDistance, yDistance, 0);
    }

    // If the button for click is pressed:
    if (clickState == HIGH) {

        // If the USB mouse is not yet pressed, press it:
        if (!Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.press(MOUSE_LEFT);
        }
        
    }

    // Else the button for click is not pressed:
    else {

        // If the USB mouse is still pressed, release it:
        if (Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.release(MOUSE_LEFT);
        }

    }

    // Delay so the mouse doesn't move too fast:
    delay(responseDelay);
}