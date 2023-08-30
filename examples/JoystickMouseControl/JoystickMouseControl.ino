/*
    JoystickMouseControl

    Controls the mouse from a joystick on an Arduino UNO R3, or other
    ATmega328 based board.

    Uses a push-button to turn on and off mouse control, and a second push-button
    to click the left mouse button.

    Hardware:
        - VUSB circuit, connected to D2, D4 and D5
                TODO: link to vusb wiring

        - 2-axis joystick connected to pins A0 and A1

        - Push-buttons connected to pin D8 and D9
                NOTE: The pins differ from those mentioned in the tutorial link below.

    The mouse movement is always relative. This sketch reads two analog inputs
    that range from 0 to 1023 (or less on either end) and translates them into
    ranges of -6 to 6.
    The sketch assumes that the joystick resting values are around the middle of
    the range, but that they vary within a threshold.

    WARNING: When you use the Mouse.move() command, the Arduino takes over your
    mouse! Make sure you have control before you use the command. This sketch
    includes a push-button to toggle the mouse control state, so you can turn on
    and off mouse control.

    Created 15 Sep 2011
    Updated 28 Mar 2012
    by Tom Igoe

    Modified 20 Jul 2023
    for unoHID library

    This example code is in the public domain.

    https://www.arduino.cc/en/Tutorial/BuiltInExamples/JoystickMouseControl
*/

#include "unoHID.h"

// Set pin numbers for switch, joystick axes, and LED:
const int switchPin = 8;    // Switch to turn on and off mouse control
const int mouseButton = 9;  // Input pin for the mouse push-button
const int xAxis = A0;       // Joystick X axis
const int yAxis = A1;       // Joystick Y axis
const int ledPin = 13;       // Mouse control LED

// Parameters for reading the joystick:
int range = 12;             // Output range of X or Y movement
int responseDelay = 5;      // Response delay of the mouse, in ms
int threshold = range / 4;  // Resting threshold
int center = range / 2;     // Resting position value

bool mouseIsActive = false;  // Whether or not to control the mouse
int lastSwitchState = LOW;   // Previous switch state

// Functions
int readAxis(int thisAxis);


void setup() {
    pinMode(switchPin, INPUT);  // The switch pin
    pinMode(ledPin, OUTPUT);    // The LED pin

    // Take control of the mouse:
    Mouse.begin();
}


void loop() {
    // Read the switch:
    int switchState = digitalRead(switchPin);

    // If it's changed and it's high, toggle the mouse state:
    if (switchState != lastSwitchState) {
        if (switchState == HIGH) {
            mouseIsActive = !mouseIsActive;
            digitalWrite(ledPin, mouseIsActive);    // Turn on LED to indicate mouse state
        }
    }

    // Save switch state for next comparison:
    lastSwitchState = switchState;

    // Read and scale the two axes:
    int xReading = readAxis(A0);
    int yReading = readAxis(A1);

    // If the mouse control state is active, move the mouse:
    if (mouseIsActive) {
        Mouse.move(xReading, yReading, 0);
    }

    // Read the mouse button and click or not click:

    // If the button for click is pressed:
    if (digitalRead(mouseButton) == HIGH) {

        // If the USB mouse is not yet pressed, press it:
        if (!Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.press(MOUSE_LEFT);
        }

    }

    // Else the button for click is not pressed
    else {

        // If the mouse is pressed, release it:
        if (Mouse.isPressed(MOUSE_LEFT)) {
            Mouse.release(MOUSE_LEFT);
        }

    }

    delay(responseDelay);
}

/*
Reads an axis (0 or 1 for x or y) and scales the analog input range to a range
from 0 to <range>
*/

int readAxis(int thisAxis) {
    // Read the analog input:
    int reading = analogRead(thisAxis);

    // Map the reading from the analog input range to the output range:
    reading = map(reading, 0, 1023, 0, range);

    // If the output reading is outside from the rest position threshold, use it:
    int distance = reading - center;

    if (abs(distance) < threshold) {
        distance = 0;
    }

    // Return the distance for this axis:
    return distance;
}