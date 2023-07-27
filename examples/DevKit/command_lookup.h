/* 
    Devkit
    -------------------------------------------
    File: command_lookup.h

    - Table of commands, as both Enum and String.
    - Function to look-up Enum value by String.
*/

#ifndef __COMMAND_LOOKUP_H__
#define __COMMAND_LOOKUP_H__

#include "unoHID.h"

// Used to identify parsed commands
// --------------------------------
enum Command : uint8_t {
    HELP,
    CONSTANTS,
    DELAY,

    MOUSE_BEGIN,
    MOUSE_END,
    MOUSE_MOVE,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_CLICK,
    MOUSE_ISPRESSED,
    MOUSE_DOUBLECLICK,
    MOUSE_LONGCLICK,
    MOUSE_SCROLL,
    MOUSE_SETTXDELAY,
    
    KEYBOARD_BEGIN,
    KEYBOARD_END,
    KEYBOARD_PRESS,
    KEYBOARD_PRINT,
    KEYBOARD_PRINTLN,
    KEYBOARD_RELEASE,
    KEYBOARD_RELEASEALL,
    KEYBOARD_WRITE,
    KEYBOARD_SETTXDELAY
};

// Strings entered on command-line
// Order matches the enum
// -------------------------------
PROGMEM const char COMMANDTABLE_KEYS[][20] = {
    "help",
    "constants",
    "delay",

    "Mouse.begin",
    "Mouse.end",
    "Mouse.move",
    "Mouse.press",
    "Mouse.release",
    "Mouse.click",
    "Mouse.isPressed",
    "Mouse.doubleClick",
    "Mouse.longClick",
    "Mouse.scroll",
    "Mouse.setTxDelay",

    "Keyboard.begin",
    "Keyboard.end",
    "Keyboard.press",
    "Keyboard.print",
    "Keyboard.println",
    "Keyboard.release",
    "Keyboard.releaseAll",
    "Keyboard.write",
    "Keyboard.setTxDelay"
};

// Find the enum which matches a command-line string
// -------------------------------------------------
bool lookupCommand(char *query, Command *result) {
    for(uint8_t i = 0; i < sizeof(COMMANDTABLE_KEYS) / sizeof(COMMANDTABLE_KEYS[0]); i++) {
        // Grab the key string from pgm
        char key[20];
        strcpy_P(key, (char *)(COMMANDTABLE_KEYS[i]));

        // Check for match
        if (strcmp(query, key) == 0) {
            *result = (Command) i;
            return true;    // Success
        }
    }
    return false;   // Not found   
}


#endif