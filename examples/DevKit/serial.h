#ifndef __DEVKIT_SERIAL__
#define __DEVKIT_SERIAL__

#include <Arduino.h>

// Get command(s) from serial
void get_serial(char *serial_data, uint16_t buffer_len) {
    unsigned int cursor = 0;

    Serial.print(">> ");

    while (true) {
        // Grab all available bytes
        char next_char = 0;
        while (Serial.available() && cursor < buffer_len - 1) {    //Leave room for null char
            next_char = (char) Serial.read();
            Serial.print(next_char);    //Echo back

            if (next_char == '\b') {  // Handle backspace
                if(cursor > 0) {
                    cursor = max(0, cursor - 1);
                    Serial.print(" \b");  //Overwrite the mistake
                }
                else Serial.print(" "); //If cmdline empty, refuse to backspace;
            }

            else if (next_char == ';') {  // If multi line command
                Serial.read();  //Clear next character - expecting space
                serial_data[cursor] = next_char;
                cursor++;
            }
            

            else if (next_char == '\n')  // If end of a command line 
                break;

            else if (next_char == '\r' || next_char == ')') {} //Ignore carriage return

            else { // If data is part of a command
                serial_data[cursor] = next_char;
                cursor++;
            }
        }

        if (next_char == '\n') break; // Propogating break from inner loop
    }

    serial_data[cursor] = '\0';  // Null terminator added
}

#endif