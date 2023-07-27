/* 
    Devkit
    -------------------------------------------
    File: serial.h

    - Function to read serial, process slightly,
      and store in char[]
*/

#ifndef __DEVKIT_SERIAL__
#define __DEVKIT_SERIAL__

#include <Arduino.h>

// Get command(s) from serial
// -----------------------------------------------------
void get_serial(char *serial_data, uint16_t buffer_len) {
    unsigned int cursor = 0;

    // Prompt user
    Serial.print(">> ");

    // Allow user to enter special characters when part of a string
    bool in_brackets = false;
    bool in_quotes = false;
    bool in_double_quotes = false;

    // Keep grabbing serial input - loop will break internally
    while (true) {

        // Loop over each incoming character, until we run out, or our output is full
        char next_char = 0;
        while (Serial.available() && cursor < buffer_len - 1) {    // ( "-1": Leave room for null terminator)
            next_char = (char) Serial.read();
            Serial.print(next_char);    // Echo back to serial monitor

            // Handle characters appropriately
            switch (next_char) {

                // Backspace
                // ---------
                case '\b':
                    if(cursor > 0) {
                        cursor = max(0, cursor - 1);

                        // Check if we're still in quotes
                        if(serial_data[cursor] == '\'')
                            in_quotes = !in_quotes;
                        else if(serial_data[cursor] == '\"')
                            in_double_quotes = !in_double_quotes;

                        serial_data[cursor] = '\0';
                        Serial.print(" \b");  //Overwrite the mistake
                    }
                    else 
                        Serial.print(" "); //If cmdline empty, refuse to backspace;
                    break;

            
                case '(':
                    // Detect start of argument brackets
                    if (!in_quotes && !in_double_quotes) {
                        in_brackets = true;
                        // If previous character was a space, delete it
                        if ( cursor > 0 && serial_data[cursor-1] == ' ' )
                            cursor--;
                    }
                    // Store the char
                    serial_data[cursor] = '(';
                    cursor++;
                    break;


                case ')':
                    // If closing argument brackets
                    if (!in_quotes && !in_double_quotes) {
                        in_brackets = false;
                    }
                    // Store the char
                    serial_data[cursor] = ')';
                    cursor++;    
                    break;


                // Single quote
                // -------------
                case '\'':
                    // Mark as a char literal
                    in_quotes = !in_quotes;
                    // Store the char
                    serial_data[cursor] = '\'';
                    cursor++;   
                    break;

                // Double quote
                // ------------
                case '\"':
                    // Mark as a string literal
                    in_double_quotes = !in_double_quotes;
                    // Store the char
                    serial_data[cursor] = '\"';
                    cursor++;                      
                    break;


                case ';':
                    // If semicolon indicated end of a command
                    if(!in_brackets) {
                        // Swap for a newline, so we can split commands, leaving semicolon containing strings alone
                        serial_data[cursor] = '\n';
                        cursor++;
                    }
                    else {
                        // Store the char
                        serial_data[cursor] = ';';
                        cursor++;
                    }
                    break;


                case ',':
                    // If comma seperates arguments
                    if (!in_quotes && !in_double_quotes && cursor > 0) {
                        // Swap for carriage return, so we can split arguments later, leaving comma containing strings alone
                        serial_data[cursor] = '\r';
                        cursor++;
                    }
                    else {
                        // Store che char
                        serial_data[cursor] = ',';
                        cursor++;
                    }
                    break;


                case '\n':
                    // If end of a command line, leave this loop
                    // The newline char will then also break the outer loop
                    // This signals the end of input, returning the method
                    break;


                case '\r':
                    // Ignore carriage return
                    // We're using the char to mark arguments, and it has no use in our serial input
                    break;


                default:
                    // All other characters
                    // Store as normal
                    serial_data[cursor] = next_char;
                    cursor++;
                    break;

            }   // End - character handling switch
                
            if (next_char == '\n') break; // Propogating break from inner loop (superfluous, but better to be safe)
        }   // End - while serial available

        if (next_char == '\n') break; // Propogating break from inner loop
    }   // End - infinite loop

    serial_data[cursor] = '\0';  // Null terminator added
    Serial.println();   // Blank line to serial monitor before command output

    // serial_data[] is ready for use. Subroutine complete
}

#endif