// DO NOT CONNECT 5V TO TARGET DEVICE
// See https://github.com/todd-herbert/unoHID/blob/main/docs/self-powered/warning.md

/*========================================================================
                        _____             _  ___ _   
                       |  __ \           | |/ (_) |  
                       | |  | | _____   _| ' / _| |_ 
                       | |  | |/ _ \ \ / /  < | | __|
                       | |__| |  __/\ V /| . \| | |_ 
                       |_____/ \___| \_/ |_|\_\_|\__|
                                                     

 This sketch is a tool for testing unoHID commands and coordinates,
 without needing to rebuild over and over.

 Flash the sketch to your Arduino and open a serial monitor (9600 baud)
 to access a commandline for executing unoHID commands.

    _________________________________________________________________
    | >> Mouse.click();                                              |
    |                                                                |
    |     - Clicking button: MOUSE_LEFT                              |
    |                                                                |
    | >> delay(1000); Keyboard.print("Hi");                          |
    |                                                                |
    |     - Delaying 1000ms                                          |
    |                                                                |
    |     - Keyboard printing "Hi"                                   |
    |                                                                |
    | >>                                                             |
    | _______________________________________________________________|


==========================================================================*/



#define INPUT_MAX_LENGTH 500

#include <Arduino.h>
#include "unoHID.h"

#include "serial.h"
#include "parse.h"
#include "execute.h"
#include "help.h"

void setup() {
    delay(1000);
    Serial.begin(9600);

    Serial.println();
    Serial.println(F("==================================================="));
    Serial.println(F("                   unoHID devkit                   "));
    Serial.println(F("==================================================="));
    Serial.println();
    Serial.println(F("type \"help\" for a list of commands"));
    Serial.println(F("type \"constants\" for a list of special keys"));
    Serial.println();
    Serial.println(F("\"Mouse.begin()\" or \"Keyboard.begin()\" to start USB"));
    Serial.println();

}



char serial_data [INPUT_MAX_LENGTH];
bool connected = false;

void loop() {
    // Pointer gets moved during argument splitting (strtok); align it again.
    char* serial_data_pointer = (char*)serial_data;

    // Get raw command from serial
    get_serial(serial_data_pointer, INPUT_MAX_LENGTH);

    // Parse the serial, and take appropriate action
    ParsedCommand parsed;
    while ( parse(serial_data_pointer, &parsed) ) {
        execute(&parsed);
    }
}
