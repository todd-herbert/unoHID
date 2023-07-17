#ifndef __DEVKIT_HELP__
#define __DEVKIT_HELP__

#include <Arduino.h>

void print_help() {
    Serial.println();
    #ifdef EMULATE_MOUSE 
        Serial.println(F("Available commands (EMULATE_MOUSE):"));
    #endif
    #ifdef EMULATE_STYLUS
        Serial.println(F("Available commands (EMULATE_STYLUS):"));
    #endif
    #ifdef EMULATE_HYBRID
        Serial.println(F("Available commands (EMULATE_HYBRID):"));
    #endif

    Serial.println();
    Serial.println(F("begin"));
    Serial.println(F("delay"));
    Serial.println(F("set_dimensions"));
    Serial.println(F("click"));
    Serial.println(F("right_click"));
    Serial.println(F("double_click"));
    Serial.println(F("long_press"));
    Serial.println(F("drag"));
    Serial.println(F("hold"));
    Serial.println(F("travel"));
    Serial.println(F("release"));
    Serial.println(F("scroll"));
    
    #if defined(EMULATE_MOUSE) || defined(EMULATE_HYBRID)
        Serial.println(F("middle_click"));
        Serial.println(F("position"));
    #endif
    #ifdef EMULATE_STYLUS
        Serial.println(F("blink"));
    #endif

    Serial.println();
    Serial.println(F("For more information: help <command>"));
}

void print_help(char *command) {
    if(strcmp(command, "begin") == 0) {
        Serial.println(F("begin  <screen_width> <screen_height>"));
        Serial.println(F("\nBegins a new USB connection. \nUsually only required when a device is not powered directly from its Host USB port \n(i.e. during this serial debugging session)\nOptionally, pass <screen_width> and <screen_height> now. See \"help\" set_dimensions"));
    }
    else if(strcmp(command, "delay") == 0) {
        Serial.println(F("delay <duration>"));
        Serial.println(F("pause for <duration> milliseconds"));
    }
    else if(strcmp(command, "set_dimensions") == 0) {
        Serial.println(F("set_dimensions <width> <height>"));
        Serial.println(F("\nSet the width and height (integer) that will be used to calculate cursor position. \nNote: The full screen width and height will always be used. \nThese values are adjustable for ease of program design: consider them to pixels, millimetes, or percentage."));
    }
    else if(strcmp(command, "click") == 0) {
        Serial.println(F("click <x> <y>"));
        Serial.println(F("\nPerform a single click at point (x,y)"));
    }
    else if(strcmp(command, "right_click") == 0) {
        Serial.println(F("right_click <x> <y>"));
        Serial.println(F("\nPerform a right click at point (x,y)"));
    }
    else if(strcmp(command, "double_click") == 0) {
        Serial.println(F("double_click <x> <y>"));
        Serial.println(F("\nPerform a double click at point (x,y)"));
    }
    else if(strcmp(command, "long_press") == 0) {
        Serial.println(F("long_press <x> <y> <duration>"));
        Serial.println(F("\nPress at point (x,y) for <duration> milliseconds"));
    }
    else if(strcmp(command, "drag") == 0) {
        Serial.println(F("drag <from x> <from y> <to x> <to y> <duration>"));
        Serial.println(F("\nPerform a click and drag maneuver, with duration in ms.\nMinimum duration 100ms"));
    }
    else if(strcmp(command, "hold") == 0) {
        Serial.println(F("hold <x> <y>"));
        Serial.println(F("\nPress and hold indefinitely"));
        Serial.println(F("Make sure to follow with \"release\"!"));
    }
    else if(strcmp(command, "travel") == 0) {
        Serial.println(F("travel <from x> <from y> <to x> <to y> <duration>"));
        Serial.println(F("\nGradually move the cursor from point to point, with duration in ms.\nMinimum duration 100ms"));
        Serial.println(F("Useful with \"hold\" and \"release\""));
    }
    else if(strcmp(command, "release") == 0) {
        Serial.println(F("release"));
        Serial.println(F("\nStop an indefinite press started with \"hold\""));
    }
    else if(strcmp(command, "scroll") == 0) {
        Serial.println(F("scroll <x> <y> <amount>"));
        Serial.println(F("\nPlace the cursor at point (x,y), then scroll by <amount> using the wheel."));
        Serial.println(F("\nThe distance scrolled is consistent, but somewhat arbitrary. You will need to manually determine the correct value."));
    }

    #if defined(EMULATE_MOUSE) || defined(EMULATE_HYBRID)
        else if(strcmp(command, "position") == 0) {
            Serial.println(F("position <x> <y>"));
            Serial.println(F("\nMove the cursor to point (x,y)"));
            Serial.println(F("This low level command is not required for most operation, but may be helpful in certain situations."));
        }
        else if(strcmp(command, "middle_click") == 0) {
            Serial.println(F("middle_click <x> <y>"));
            Serial.println(F("\nPress the middle button (scroll wheel) at point (x,y)"));
        }
    #endif
    #ifdef EMULATE_STYLUS
        else if(strcmp(command, "blink") == 0) {
            Serial.println(F("blink <x> <y>"));
        Serial.println(F("\nForce the cursor to briefly display, for debugging."));
        }
    #endif
}
#endif