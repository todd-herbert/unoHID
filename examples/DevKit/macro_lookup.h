/* 
    Devkit
    -------------------------------------------
    File: command_lookup.h

    - Enum of keyboard layouts
    - Table of constants, as both Enum and int value.
    - Function to look-up int value by String.
    - Function to look-up string value by int.
*/

#ifndef __MACRO_LOOKUP_H__
#define __MACRO_LOOKUP_H__

#include "unoHID.h"

enum KEYBOARDLAYOUT_MACROS {
    KEYBOARDLAYOUT_DA_DK,
    KEYBOARDLAYOUT_DE_DE,
    KEYBOARDLAYOUT_EN_US,
    KEYBOARDLAYOUT_ES_ES,
    KEYBOARDLAYOUT_FR_FR,
    KEYBOARDLAYOUT_IT_IT,
    KEYBOARDLAYOUT_SV_SE
};

PROGMEM const char MACROTABLE_KEYS[][21] = {
    "KEY_LEFT_CTRL",
    "KEY_LEFT_SHIFT",
    "KEY_LEFT_ALT",
    "KEY_LEFT_GUI",
    "KEY_RIGHT_CTRL",
    "KEY_RIGHT_SHIFT",
    "KEY_RIGHT_ALT",
    "KEY_RIGHT_GUI",
    "KEY_UP_ARROW",
    "KEY_DOWN_ARROW",
    "KEY_LEFT_ARROW",
    "KEY_RIGHT_ARROW",
    "KEY_BACKSPACE",
    "KEY_TAB",
    "KEY_RETURN",
    "KEY_MENU",
    "KEY_ESC",
    "KEY_INSERT",
    "KEY_DELETE",
    "KEY_PAGE_UP",
    "KEY_PAGE_DOWN",
    "KEY_HOME",
    "KEY_END",
    "KEY_CAPS_LOCK",
    "KEY_PRINT_SCREEN",
    "KEY_SCROLL_LOCK",
    "KEY_PAUSE",
    "KEY_NUM_LOCK",
    "KEY_KP_SLASH",
    "KEY_KP_ASTERISK",
    "KEY_KP_MINUS",
    "KEY_KP_PLUS",
    "KEY_KP_ENTER",
    "KEY_KP_1",
    "KEY_KP_2",
    "KEY_KP_3",
    "KEY_KP_4",
    "KEY_KP_5",
    "KEY_KP_6",
    "KEY_KP_7",
    "KEY_KP_8",
    "KEY_KP_9",
    "KEY_KP_0",
    "KEY_KP_DOT",
    "KEY_F1",
    "KEY_F2",
    "KEY_F3",
    "KEY_F4",
    "KEY_F5",
    "KEY_F6",
    "KEY_F7",
    "KEY_F8",
    "KEY_F9",
    "KEY_F10",
    "KEY_F11",
    "KEY_F12",
    "KEY_F13",
    "KEY_F14",
    "KEY_F15",
    "KEY_F16",
    "KEY_F17",
    "KEY_F18",
    "KEY_F19",
    "KEY_F20",
    "KEY_F21",
    "KEY_F22",
    "KEY_F23",
    "KEY_F24",

    "MOUSE_LEFT",
    "MOUSE_RIGHT",
    "MOUSE_MIDDLE",
    
    "KeyboardLayout_da_DK",
    "KeyboardLayout_de_DE",
    "KeyboardLayout_en_US",
    "KeyboardLayout_es_ES",
    "KeyboardLayout_fr_FR",
    "KeyboardLayout_it_IT",
    "KeyboardLayout_sv_SE",
};

PROGMEM const uint16_t MACROTABLE_VALS[] = {
    KEY_LEFT_CTRL,
    KEY_LEFT_SHIFT,
    KEY_LEFT_ALT,
    KEY_LEFT_GUI,
    KEY_RIGHT_CTRL,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_ALT,
    KEY_RIGHT_GUI,
    KEY_UP_ARROW,
    KEY_DOWN_ARROW,
    KEY_LEFT_ARROW,
    KEY_RIGHT_ARROW,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_RETURN,
    KEY_MENU,
    KEY_ESC,
    KEY_INSERT,
    KEY_DELETE,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_PRINT_SCREEN,
    KEY_SCROLL_LOCK,
    KEY_PAUSE,
    KEY_NUM_LOCK,
    KEY_KP_SLASH,
    KEY_KP_ASTERISK,
    KEY_KP_MINUS,
    KEY_KP_PLUS,
    KEY_KP_ENTER,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_0,
    KEY_KP_DOT,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,

    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,

    KEYBOARDLAYOUT_DA_DK,
    KEYBOARDLAYOUT_DE_DE,
    KEYBOARDLAYOUT_EN_US,
    KEYBOARDLAYOUT_ES_ES,
    KEYBOARDLAYOUT_FR_FR,
    KEYBOARDLAYOUT_IT_IT,
    KEYBOARDLAYOUT_SV_SE
};

bool lookupMacro(char *query, uint16_t *result) {
    for(uint8_t i = 0; i < sizeof(MACROTABLE_VALS) / sizeof (uint16_t); i++) {
        // Grab the key string from pgm
        char key[ sizeof(MACROTABLE_KEYS) / (sizeof(MACROTABLE_VALS) / sizeof (uint16_t)) ];
        strcpy_P(key, (char *)(MACROTABLE_KEYS[i]));

        // Check for match
        if (strcmp(query, key) == 0) {
            *result = pgm_read_word_near(MACROTABLE_VALS + i);        
            return true;    // Success
        }
    }
    return false;   // Not found   
}

// (Hopefully) find the string representation of a macro
bool reverseLookupMacro(uint16_t query, char** result) {
    for(uint8_t i = 0; i < sizeof(MACROTABLE_VALS) / sizeof (uint16_t); i++) {
        // Grab the next macro value from table
        uint16_t test_match = pgm_read_word_near(MACROTABLE_VALS + i);   
        
        // Check for match
        if (query == test_match) {
            *result = new char[ sizeof(MACROTABLE_KEYS) / (sizeof(MACROTABLE_VALS) / sizeof (uint16_t)) ];
            strcpy_P(*result, (char *)(MACROTABLE_KEYS[i]));
            // result = (char**)MACROTABLE_KEYS[i];

            return true;
        }
    }
    return false; // Not found
}

#endif