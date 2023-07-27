/*
    ButtonMouseControl

    Controls the mouse from five pushbuttons on an Arduino UNO R3, or other
    ATmega328 based board.

    Hardware:
        - VUSB circuit, connected to D2, D4 and D5
                TODO: link to vusb wiring

        - five pushbuttons attached to D8, D9, D10, D11, D12
                NOTE: The pins differ from those mentioned in the attached tutorial.

    The mouse movement is always relative. This sketch reads four pushbuttons, and
    uses them to set the movement of the mouse.

    WARNING: When you use the Mouse.move() command, the Arduino takes over your
    mouse! Make sure you have control before you use the mouse commands.

    created 15 Mar 2012
    modified 27 Mar 2012
    by Tom Igoe

    modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.


    https://www.arduino.cc/en/Tutorial/ButtonMouseControl

*/

#include "unoHID.h"

// set pin numbers for the five buttons:
const int upButton = 8;
const int downButton = 9;
const int leftButton = 10;
const int rightButton = 11;
const int mouseButton = 12;

int range = 5;              // output range of X or Y movement; affects movement speed
int responseDelay = 10;     // response delay of the mouse, in ms

void setup() {

  // initialize the buttons' inputs:

  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(mouseButton, INPUT);

  // initialize mouse control:

  Mouse.begin();
}

void loop() {

    // read the buttons:

    int upState = digitalRead(upButton);
    int downState = digitalRead(downButton);
    int rightState = digitalRead(rightButton);
    int leftState = digitalRead(leftButton);
    int clickState = digitalRead(mouseButton);

    // calculate the movement distance based on the button states:

    int xDistance = (rightState - leftState) * range;
    int yDistance = (downState- upState) * range;

    // if X or Y is non-zero, move:

    if ((xDistance != 0) || (yDistance != 0)) {
        Mouse.move(xDistance, yDistance, 0);
    }

    // if the mouse button is pressed:
    if (clickState == HIGH) {

        // if the mouse is not pressed, press it:
        if (!Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.press(MOUSE_LEFT);
        }
        
    }

    // else the mouse button is not pressed:
    else {
        // if the mouse is pressed, release it:
        if (Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.release(MOUSE_LEFT);
        }

    }

    // a delay so the mouse doesn't move too fast:

    delay(responseDelay);
}