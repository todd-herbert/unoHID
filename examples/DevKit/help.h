/* 
    Devkit
    ---------------------------------------------------------
    File: help.h

    - Functions for printing tidy serial output
    - Function to list all commands
    - Function to list all constants
    - Functions to provide information on specific commands
*/

#ifndef __DEVKIT_HELP__
#define __DEVKIT_HELP__

#include <Arduino.h>
#include "unoHID.h"

#include "command_lookup.h"
#include "macro_lookup.h"

// Print an indented line of text
#define indent_4(text) Serial.println(F("    " text))

// Save a little memory
void underline() {
    indent_4("----------------------------------");
    Serial.println();
}

// List all commands
void print_help() {
    Serial.println();
    indent_4("Available commands:");
    underline();
    indent_4("constants");
    indent_4("delay()");
    Serial.println();
    indent_4("Mouse.begin()");
    indent_4("Mouse.click()");
    indent_4("Mouse.end()");
    indent_4("Mouse.move()");
    indent_4("Mouse.press()");
    indent_4("Mouse.release()");
    indent_4("Mouse.isPressed()");
    indent_4("Mouse.doubleClick()");
    indent_4("Mouse.longClick()");
    indent_4("Mouse.scroll()");
    indent_4("Mouse.setTxDelay()");
    Serial.println();
    indent_4("Keyboard.begin()");
    indent_4("Keyboard.end()");
    indent_4("Keyboard.press()");
    indent_4("Keyboard.print()");
    indent_4("Keyboard.println()");
    indent_4("Keyboard.release()");
    indent_4("Keyboard.releaseAll()");
    indent_4("Keyboard.write()");
    indent_4("Keyboard.setTxDelay()");
    Serial.println();
    indent_4("For more information: help(command)");
    indent_4("Example: help(Mouse.begin)");
}

// Give help for a specific command
void print_help(Command command) {
    
    Serial.println();   // Extra line at start

    switch (command) {
        case CONSTANTS:
            indent_4("constants");
            underline();
            indent_4("In DevKit only, outputs a list of constant expressions,");
            indent_4("for special keys, mouse buttons and keyboard layouts.");
            break;

        case DELAY:
            indent_4("delay(duration)");
            underline();
            indent_4("Pause for the specified timespan");
            indent_4("duration: length of yause, in milliseconds");
            break;
        case MOUSE_BEGIN:
            indent_4("Mouse.begin()");
            underline();
            indent_4("Begin mouse emulation");        
            break;
        case MOUSE_CLICK:
            indent_4("Mouse.click()");
            indent_4("Mouse.click( button );");
            underline();
            indent_4("button (optional) : mouse button to use.");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_END:
            indent_4("Mouse.end()");
            underline();
            indent_4("Stops emulating the mouse connected to a computer. To start control, use Mouse.begin()");
            break;
        case MOUSE_MOVE:
            indent_4("Mouse.move(x, y)");
            indent_4("Mouse.move(x, y, wheel)");
            underline();
            indent_4("Moves the cursor. Motion onscreen is relative to cursor's current location.");
            indent_4("x: amount to move, horizontally. (Positive values move right)");
            indent_4("y: amount to move, vertically. (Positive values move down");
            indent_4("wheel (optional): amount to move mouse wheel. (Positive values roll wheel forwards)");
            break;
        case MOUSE_PRESS:
            indent_4("Mouse.press()");
            indent_4("Mouse.press(button)");
            underline();
            indent_4("Press is the equivalent of clicking and continuously holding the mouse button. A press is cancelled with Mouse.release().");
            Serial.println();
            indent_4("button (optional): which mouse button to press");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_RELEASE:
            indent_4("Mouse.release()");
            indent_4("Mouse.release(button)");
            underline();
            indent_4("Release a previously pressed button.");
            Serial.println();
            indent_4("button (optional): which mouse button to release");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_ISPRESSED:
            indent_4("Mouse.isPressed()");
            indent_4("Mouse.isPressed(button)");
            underline();
            indent_4("Returns current status of a mouse button.");
            Serial.println();
            indent_4("button (optional): which mouse button to release");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_DOUBLECLICK:
            indent_4("Mouse.doubleClick()");
            indent_4("Mouse.doubleClick(button)");
            underline();
            indent_4("Double click at the location of the cursor.");
            Serial.println();
            indent_4("button (optional): which mouse button to release");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_LONGCLICK:
            indent_4("Mouse.longClick(duration)");
            indent_4("Mouse.longClick(duration, button)");
            underline();
            indent_4("At the location of the cursor, press and hold a mouse button for specified duration.");
            Serial.println();
            indent_4("duration: how long to hold, in milliseconds");
            indent_4("button (optional): which mouse button to release");
            indent_4("Possible values:");
            indent_4(" - MOUSE_LEFT (default)");
            indent_4(" - MOUSE_RIGHT");
            indent_4(" - MOUSE_MIDDLE");
            break;
        case MOUSE_SCROLL:
            indent_4("Mouse.scroll(amount)");
            underline();
            indent_4("Scroll a long distance, using mouse wheel");
            Serial.println();
            indent_4("amount: How far to scroll. Positive values roll mouse wheel forwards.");
            break;
        case MOUSE_SETTXDELAY:
            indent_4("Mouse.setTxDelay(duration)");
            underline();
            indent_4("Specify a small delay to be added after every mouse command, for rate limiting.");
            Serial.println();
            indent_4("duration: how long, in milliseconds, to pause");
            break;


        case KEYBOARD_BEGIN:
            indent_4("Keyboard.begin()");
            indent_4("Keyboard.begin(layout)");
            underline();
            indent_4("Start emulating a keyboard connected to a computer.");
            indent_4("layout(optional): the keyboard layout to use.");
            indent_4("Possible options:");
            indent_4(" - KeyboardLayout_da_DK");
            indent_4(" - KeyboardLayout_de_DE");
            indent_4(" - KeyboardLayout_en_US");
            indent_4(" - KeyboardLayout_fr_FR");
            indent_4(" - KeyboardLayout_it_IT");
            indent_4(" - KeyboardLayout_sv_SE");
            break;
        case KEYBOARD_END:
            indent_4("Keyboard.end()");
            underline();
            indent_4("Stops the keyboard emulation.");
            break;
        case KEYBOARD_PRESS:
            indent_4("Keyboard.press(key)");
            underline();
            indent_4("Press and hold a key. Useful with modifier keys.");
            indent_4("To end key press, use \"Keyboard.release()\" or \"Keyboard.releaseAll()\"");
            indent_4("button: the key to press");
            break;
        case KEYBOARD_PRINT:
            indent_4("Keyboard.print( character(s) )");
            underline();
            indent_4("Sends one or more keystrokes to a connected computer.");
            indent_4("character(s): a char or string to print");
            break;
        case KEYBOARD_PRINTLN:
            indent_4("Keyboard.println( character(s) )");
            underline();
            indent_4("Sends one or more keystrokes to a connected computer,");
            indent_4("followed by a keystroke on the Enter key.");
            indent_4("character(s): a char or string to print");
            break;
        case KEYBOARD_RELEASE:
            indent_4("Keyboard.release(key)");
            underline();
            indent_4("Lets go of a held key. See Keyboard.press()");
            indent_4("key: the key to release");
            break;
        case KEYBOARD_RELEASEALL:
            indent_4("Keyboard.releaseAll()");
            underline();
            indent_4("Lets go of all keys currently pressed. See Keyboard.press()");
            break;
        case KEYBOARD_WRITE:
            indent_4("Keyboard.write(character)");
            underline();
            indent_4("Sends a keystroke to connected computer. You can send some ASCII characters,");
            indent_4("or the additional keyboard modifiers and special keys.");
            indent_4("Only ASCII characters that are on the keyboard are supported");
            indent_4("character: a char, int, or macro to be sent to the computer.");
            break;
        case KEYBOARD_SETTXDELAY:
            indent_4("Keyboard.setTxDelay(duration)");
            underline();
            indent_4("Specify a small delay to be added after every keyboard command, for rate limiting.");
            Serial.println();
            indent_4("duration: how long, in milliseconds, to pause");
            break;            

        default:
            Serial.println("Command not found");
            break;     
    }

    Serial.println();   // Extra line at end
}

// List available macros
void print_macros() {
    Serial.println();
    indent_4("Constants:");
    underline();

    // Grab each macro from progmem and dump it into serial
    char macro_string[ sizeof(MACROTABLE_KEYS) / (sizeof(MACROTABLE_VALS) / sizeof (uint16_t)) ];
    for(uint8_t i = 0; i < sizeof(MACROTABLE_VALS) / sizeof (uint16_t); i++) {
        strcpy_P(macro_string, (char *)(MACROTABLE_KEYS[i]));
        Serial.print("    ");
        Serial.println(macro_string);
    }

    Serial.println();
}

#undef indent_4

#endif