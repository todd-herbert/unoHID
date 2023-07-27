/* 
    Devkit
    ----------------------------------------------------------
    File: execute.h

    - Functions for printing information to serial
    - A very large switch(), handling each command seperately..
*/

#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include <Arduino.h>
#include "unoHID.h"

#include "parse.h"
#include "command_lookup.h"
#include "macro_lookup.h"
#include "help.h"

// Generic error
void complain() {
    Serial.println(F("Error - Invalid argument"));
    Serial.println();
}

void complain_type() {
    Serial.println(F("Error - Invalid argument type"));
    Serial.println();
}

void complain_count() {
    Serial.println(F("Error - Incorrect number of arguments"));
    Serial.println();
}

void indent() {
    Serial.print(F("    - "));
}

bool session_initialized = false;
bool mouse_active = false;
bool keyboard_active = false;

void execute(ParsedCommand *c) {
    // Get shorter version of ArgTypes (less typing)
    // ----------------------------------------------
    const auto StringArg    =   ParsedCommand::ArgType::StringArg;
    const auto CharArg      =   ParsedCommand::ArgType::CharArg;
    const auto MacroArg     =   ParsedCommand::ArgType::MacroArg;
    const auto IntArg       =   ParsedCommand::ArgType::IntArg;
    // ----------------------------------------------

    // Remind the user to call begin()
    if (!mouse_active && 
        !keyboard_active && 
        c->command != MOUSE_BEGIN && 
        c->command != KEYBOARD_BEGIN &&
        c->command != HELP &&
        c->command != CONSTANTS) {
            
        Serial.println(F(" ----  First, call Mouse.begin() or Keyboard.begin()  ----"));
        Serial.println();
        return;
    }

    // Handle each method individually. There aren't that many.. right?
    switch(c->command) {

        case CONSTANTS:
        // -------------
            print_macros();
            break;

        case HELP:
        // -------
        switch(c->arg_count) {
            case 0:
                print_help();
                break;
            
            case 1:
                if (c->arg_type[0] != StringArg)    return complain_type();
                {   // Restrict variable scope
                    Command help_arg;
                    if ( lookupCommand(c->string_args[0], &help_arg) )
                        print_help(help_arg);
                    else
                        return complain();
                }
                break;
            
            default:
                return complain_count();
        } break;


        case DELAY:
        // ----------------
            switch(c->arg_count) {
                case 1:
                    if(c->arg_type[0] != IntArg)    return complain_type();

                    indent();
                    Serial.print(F("Delaying "));
                    Serial.print(c->int_args[0]);
                    Serial.println(F("ms"));
                    delay(c->int_args[0]);     

                    break;

                default: 
                    return complain_count();
            } break;


        case MOUSE_BEGIN:
        // ---------------
            if(mouse_active) {
                indent();
                Serial.println(F("Mouse already active"));
                Serial.println();
                return;
            }
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Mouse starting"));
                    Mouse.begin();
                    mouse_active = true;

                    break;

                default: 
                    return complain_count();
            } break;


        case MOUSE_END:
        // ---------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Mouse ending"));
                    Mouse.end();
                    mouse_active = false;

                    break;

                default: 
                    return complain_count();
            } break;

        case MOUSE_MOVE:
        // ---------------
            switch(c->arg_count) {
                case 2:
                    if(c->arg_type[0] != IntArg)    return complain_type();
                    if(c->arg_type[1] != IntArg)    return complain_type();

                    indent();
                    Serial.print(F("Mouse moved by "));
                    Serial.print(F("X: "));
                    Serial.print(c->int_args[0]);
                    Serial.print(F(", "));
                    Serial.print(F("Y: "));
                    Serial.println(c->int_args[1]);
                    Mouse.move(c->int_args[0], c->int_args[1]);

                    break;

                case 3:
                    if(c->arg_type[0] != IntArg)    return complain_type();
                    if(c->arg_type[1] != IntArg)    return complain_type();
                    if(c->arg_type[2] != IntArg)    return complain_type();

                    indent();
                    Serial.print(F("Mouse moved by "));
                    Serial.print(F("X: "));
                    Serial.print(c->int_args[0]);
                    Serial.print(F(", "));
                    Serial.print(F("Y: "));
                    Serial.print(c->int_args[1]);
                    Serial.print(F(", "));
                    Serial.print(F("Wheel: "));
                    Serial.println(c->int_args[2]);                    
                    Mouse.move(c->int_args[0], c->int_args[1], c->int_args[2]);

                    break;
                                        
                default: 
                    return complain_count();
            } break;

        case MOUSE_PRESS:
        // ----------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Pressing button: MOUSE_LEFT"));
                    Mouse.press();
                    break;

                case 1:
                    if( c->arg_type[0] == IntArg ) { 
                        if (c->int_args[0] < 1 || c->int_args[0] > 3) return complain();
                        indent();
                        Serial.print(F("Pressing mouse button: "));
                        Serial.println(c->int_args[0]);
                        Mouse.press((MouseButton)c->int_args[0]);  
                    }
                    else if (c->arg_type[0] == MacroArg) {
                        indent();
                        if (c->macro_args[0] == MOUSE_LEFT)
                            Serial.println(F("Pressing button: MOUSE_LEFT"));
                        else if (c->macro_args[0] == MOUSE_RIGHT)
                            Serial.println(F("Pressing button: MOUSE_RIGHT"));
                        else if (c->macro_args[0] == MOUSE_MIDDLE)
                            Serial.println(F("Pressing button: MOUSE_MIDDLE"));
                        else
                            return complain();
                        
                        Mouse.press((MouseButton)c->macro_args[0]);
                    }
                    else return complain_type(); 
                    break;

                default: 
                    return complain_count();
            } break; 

        case MOUSE_RELEASE:
        // ----------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Releasing button: MOUSE_LEFT"));
                    Mouse.release();
                    break;

                case 1:
                    if( c->arg_type[0] == IntArg ) { 
                        if (c->int_args[0] < 1 || c->int_args[0] > 3) return complain();
                        indent();
                        Serial.print(F("Releasing mouse button: "));
                        Serial.println(c->int_args[0]);
                        Mouse.release((MouseButton) c->int_args[0]); 
                    }
                    else if (c->arg_type[0] == MacroArg) {
                        indent();
                        if (c->macro_args[0] == MOUSE_LEFT)
                            Serial.println(F("Releasing button: MOUSE_LEFT"));
                        else if (c->macro_args[0] == MOUSE_RIGHT)
                            Serial.println(F("Releasing button: MOUSE_RIGHT"));
                        else if (c->macro_args[0] == MOUSE_MIDDLE)
                            Serial.println(F("Releasing button: MOUSE_MIDDLE"));
                        else
                            return complain();

                        Mouse.release((MouseButton) c->macro_args[0]);
                    }
                    else return complain_type(); 
                    break;
                    
                default: 
                    return complain_count();
            } break;

        case MOUSE_CLICK:
        // ----------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Clicking button: MOUSE_LEFT"));
                    Mouse.click();
                    break;

                case 1:
                    if( c->arg_type[0] == IntArg ) { 
                        if (c->int_args[0] < 1 || c->int_args[0] > 3) return complain();
                        indent();
                        Serial.print(F("Clicking mouse button: "));
                        Serial.println(c->int_args[0]);
                        Mouse.click((MouseButton)c->int_args[0]);  
                    }
                    else if (c->arg_type[0] == MacroArg) {
                        indent();
                        if (c->macro_args[0] == MOUSE_LEFT)
                            Serial.println(F("Clicking button: MOUSE_LEFT"));
                        else if (c->macro_args[0] == MOUSE_RIGHT)
                            Serial.println(F("Clicking button: MOUSE_RIGHT"));
                        else if (c->macro_args[0] == MOUSE_MIDDLE)
                            Serial.println(F("Clicking button: MOUSE_MIDDLE"));
                        else
                            return complain();
                        
                        Mouse.click((MouseButton)c->macro_args[0]);
                    }
                    else return complain_type(); 
                    break;

                default: 
                    return complain_count();
            } break;  

        case MOUSE_ISPRESSED:
        // ----------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(Mouse.isPressed() ? "true": "false");
                    break;

                case 1:
                    if( c->arg_type[0] == IntArg ) { 
                        if (c->int_args[0] < 1 || c->int_args[0] > 3) return complain();
                        indent();
                        Serial.println(Mouse.isPressed((MouseButton)c->int_args[0]) ? "true": "false");
                    }
                    else if (c->arg_type[0] == MacroArg) {
                        indent();
                        if (c->macro_args[0] < MOUSE_LEFT || c->macro_args[0] > MOUSE_RIGHT) return complain();
                        Serial.println(Mouse.isPressed((MouseButton)c->macro_args[0]) ? "true": "false");
                    }
                    else return complain_type(); 
                    break;

                default: 
                    return complain_count();
            } break;

        case MOUSE_DOUBLECLICK:
        // ----------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Double clicking button: MOUSE_LEFT"));
                    Mouse.doubleClick();
                    break;

                case 1:
                    if( c->arg_type[0] == IntArg ) { 
                        if (c->int_args[0] < 1 || c->int_args[0] > 3) return complain();
                        indent();
                        Serial.print(F("Double clicking mouse button: "));
                        Serial.println(c->int_args[0]);
                        Mouse.doubleClick((MouseButton)c->int_args[0]);  
                    }
                    else if (c->arg_type[0] == MacroArg) {
                        indent();
                        if (c->macro_args[0] == MOUSE_LEFT)
                            Serial.println(F("Double clicking button: MOUSE_LEFT"));
                        else if (c->macro_args[0] == MOUSE_RIGHT)
                            Serial.println(F("Double clicking button: MOUSE_RIGHT"));
                        else if (c->macro_args[0] == MOUSE_MIDDLE)
                            Serial.println(F("Double clicking button: MOUSE_MIDDLE"));
                        else
                            return complain();
                        
                        Mouse.doubleClick((MouseButton)c->macro_args[0]);
                    }
                    else return complain_type(); 
                    break;

                default: 
                    return complain_count();
            } break;      

        case MOUSE_LONGCLICK:
        // ----------------
            switch(c->arg_count) {
                case 1:
                    if( c->arg_type[0] != IntArg ) return complain();
                    indent();
                    Serial.print(F("Click and hold button MOUSE_LEFT, for "));
                    Serial.print(c->int_args[0]);
                    Serial.println(F("ms"));
                    Mouse.longClick(c->int_args[0]);
                    break;

                case 2:
                    if( c->arg_type[0] != IntArg ) return complain();

                    if( c->arg_type[1] == IntArg ) { 
                        if (c->int_args[1] < 1 || c->int_args[1] > 3) return complain();
                        indent();
                        Serial.print(F("Click and hold button "));
                        Serial.print(c->int_args[1]);
                        Serial.print(F(", for "));
                        Serial.print(c->int_args[0]);
                        Serial.println(F(" ms"));
                        Mouse.longClick(c->int_args[0], (MouseButton)c->int_args[1]);  
                    }

                    else if (c->arg_type[1] == MacroArg) {
                        indent();
                        if (c->macro_args[1] == MOUSE_LEFT)
                            Serial.print(F("Click and hold button MOUSE_LEFT, for "));
                        else if (c->macro_args[1] == MOUSE_RIGHT)
                            Serial.print(F("Click and hold button MOUSE_RIGHT, for "));
                        else if (c->macro_args[1] == MOUSE_MIDDLE)
                            Serial.print(F("Click and hold button MOUSE_MIDDLE, for "));
                        else
                            return complain();

                        Serial.print(c->int_args[0]);
                        Serial.println(F(" ms"));
                        
                        Mouse.longClick(c->int_args[0], (MouseButton)c->macro_args[1]);
                    }

                    else 
                        return complain_type();
                    break;

                default: 
                    return complain_count();
            } break;   
            
        case MOUSE_SCROLL:
        // ----------------
            switch(c->arg_count) {
                case 1:
                    if(c->arg_type[0] != IntArg)    return complain();

                    indent();
                    Serial.print(F("Multiple MOUSE_WHEEL moves, for a total scroll of "));
                    Serial.println(c->int_args[0]);
                    Mouse.scroll(c->int_args[0]);     

                    break;

                default: 
                    return complain_count();
            } break;    

        case MOUSE_SETTXDELAY:
        // ----------------
            switch(c->arg_count) {
                case 1:
                    if(c->arg_type[0] != IntArg)    return complain();

                    indent();
                    Serial.print(F("From now on, leaving "));
                    Serial.print(c->int_args[0]);
                    Serial.println(F("ms between mouse updates"));
                    Mouse.setTxDelay(c->int_args[0]);     

                    break;

                default: 
                    return complain_count();
            } break;  


        case KEYBOARD_BEGIN:
        // ---------------
            if(keyboard_active) {
                indent();
                Serial.println(F("Keyboard already active"));
                Serial.println();
                return;
            }        
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Keyboard starting"));
                    Keyboard.begin();
                    keyboard_active = true;
                    break;
                
                case 1:
                    if (c->arg_type[0] != MacroArg) return complain_type();
                    if (c->macro_args[0] < KEYBOARDLAYOUT_DA_DK || c->macro_args[0] > KEYBOARDLAYOUT_SV_SE) return complain();

                    switch (c->macro_args[0]) {
                        case KEYBOARDLAYOUT_DA_DK:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_da_DK"));
                            Keyboard.begin(KeyboardLayout_da_DK);
                            break;
                        case KEYBOARDLAYOUT_DE_DE:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_de_DE"));
                            Keyboard.begin(KeyboardLayout_de_DE);
                            break;                        
                        case KEYBOARDLAYOUT_EN_US:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_en_US"));
                            Keyboard.begin(KeyboardLayout_en_US);
                            break;                        
                        case KEYBOARDLAYOUT_FR_FR:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_fr_FR"));
                            Keyboard.begin(KeyboardLayout_fr_FR);
                            break;                        
                        case KEYBOARDLAYOUT_IT_IT:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_it_IT"));
                            Keyboard.begin(KeyboardLayout_it_IT);
                            break;                        
                        case KEYBOARDLAYOUT_SV_SE:
                            Serial.print(F("Keyboard starting, with "));
                            Serial.println(F("KeyboardLayout_sv_SE"));
                            Keyboard.begin(KeyboardLayout_sv_SE);
                            break;

                        default:
                            return complain();
                    }
                    keyboard_active = true;
                    break;

                default: 
                    return complain_count();
            } break;   


        case KEYBOARD_END:
        // ---------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Keyboard ending"));
                    Keyboard.end();
                    keyboard_active = false;
                    break;

                default: 
                    return complain_count();
            } break; 


        case KEYBOARD_PRESS:
        // ---------------
            switch(c->arg_count) {
                case 1:
                    switch (c->arg_type[0]) {
                        case CharArg:
                            indent();
                            Serial.print(F("Pressing key \'"));
                            Serial.print(c->char_args[0]);
                            Serial.println('\'');
                            Keyboard.press(c->char_args[0]);
                            break;

                        case IntArg:
                            indent();
                            Serial.print(F("Pressing key with ASCII code "));
                            Serial.println(c->int_args[0]);
                            Keyboard.press((char)c->int_args[0]);
                            break;                            

                        case MacroArg:
                            // Keep variables contained
                            {
                                char* label;
                                // Try find string for the macro. Fail if not found
                                if ( !reverseLookupMacro(c->macro_args[0], &label) )    return complain();
                    
                                indent();
                                Serial.print(F("Pressing key \'"));
                                Serial.print(label);
                                Serial.println('\'');
                                Keyboard.press(c->macro_args[0]);
                                delete[] label;
                            }
                            break;                          

                        default: 
                            return complain_type();                        
                    } break;    // End type

                default: 
                    return complain_count();
            } break;    //End count


        case KEYBOARD_PRINT:
        // ---------------
            switch(c->arg_count) {
                case 1:
                    switch (c->arg_type[0]) {
                        case CharArg:
                            indent();
                            Serial.print(F("Keyboard printing \'"));
                            Serial.print(c->char_args[0]);
                            Serial.println('\'');
                            Keyboard.print(c->char_args[0]);
                            break;

                        case MacroArg:
                            // Keep variables contained
                            {
                                char* label;
                                // Try find string for the macro. Fail if not found
                                if ( !reverseLookupMacro(c->macro_args[0], &label) )    return complain();
                    
                                indent();
                                Serial.print(F("Keyboard printing \'"));
                                Serial.print(label);
                                Serial.println('\'');
                                Keyboard.print((char)c->macro_args[0]);
                                delete[] label;
                            }
                            break;

                        case StringArg:
                            // Keep variables contained
                            {                   
                                indent();
                                Serial.print(F("Keyboard printing \""));
                                Serial.print(c->string_args[0]);
                                Serial.println('\"');
                                Keyboard.print(c->string_args[0]);
                            }
                            break;

                        default: 
                            return complain_type();  
                    } break; // End type

                default: 
                    return complain_count();  
            } break; // End count


        case KEYBOARD_PRINTLN:
        // ---------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Keyboard printing a newline"));
                    Keyboard.println();
                    break;

                case 1:
                    switch (c->arg_type[0]) {
                        case CharArg:
                            indent();
                            Serial.print(F("Keyboard printing \'"));
                            Serial.print((char) c->char_args[0]);
                            Serial.println(F("\' with newline"));
                            Keyboard.println(c->char_args[0]);
                            break;

                        case MacroArg:
                            // Keep variables contained
                            {
                                char* label;
                                // Try find string for the macro. Fail if not found
                                if ( !reverseLookupMacro(c->macro_args[0], &label) )    return complain();
                    
                                indent();
                                Serial.print(F("Keyboard printing \'"));
                                Serial.print(label);
                                Serial.println(F("\' with newline"));
                                Keyboard.println((char)c->macro_args[0]);
                                delete[] label;
                            }
                            break;

                        case StringArg:
                            // Keep variables contained
                            {
                                indent();
                                Serial.print(F("Keyboard printing \""));
                                Serial.print(c->string_args[0]);
                                Serial.println(F("\" with newline"));
                                Keyboard.println(c->string_args[0]);
                            }
                            break; 

                        default: 
                            return complain_type();                        
                    } break;    // End type

                default: 
                    return complain_count();
            } break;    //End count                                                                                                      


        case KEYBOARD_RELEASE:
        // ---------------
            switch(c->arg_count) {
                case 1:
                    switch (c->arg_type[0]) {
                        case CharArg:
                            indent();
                            Serial.print(F("Releasing key \'"));
                            Serial.print(c->char_args[0]);
                            Serial.println('\'');
                            Keyboard.release(c->char_args[0]);
                            break;

                        case MacroArg:
                            // Keep variables contained
                            {
                                char* label;
                                // Try find string for the macro. Fail if not found
                                if ( !reverseLookupMacro(c->macro_args[0], &label) )    return complain();
                    
                                indent();
                                Serial.print(F("Releasing key \'"));
                                Serial.print(label);
                                Serial.println('\'');
                                Keyboard.release(c->macro_args[0]);
                                delete[] label;
                            }
                            break;

                        default: 
                            return complain_type();  
                        } break; // End type
                default: 
                    return complain_count();  
            } break;    // End count


        case KEYBOARD_RELEASEALL:
        // ---------------
            switch(c->arg_count) {
                case 0:
                    indent();
                    Serial.println(F("Releasing all keys"));
                    Keyboard.releaseAll();
                    break;

                default: 
                    return complain_count();  
            } break;    // End count


        case KEYBOARD_WRITE:
        // ---------------
            switch(c->arg_count) {
                case 1:
                    switch (c->arg_type[0]) {
                        case CharArg:
                            indent();
                            Serial.print(F("Keyboard writing \'"));
                            Serial.print(c->char_args[0]);
                            Serial.println('\'');
                            Keyboard.write(c->char_args[0]);
                            break;

                        case IntArg:
                            indent();
                            Serial.print(F("Keyboard writing ASCII code "));
                            Serial.println(c->int_args[0]);
                            Keyboard.write((char)c->int_args[0]);
                            break;

                        case MacroArg:
                            // Keep variables contained
                            {
                                char* label;
                                // Try find string for the macro. Fail if not found
                                if ( !reverseLookupMacro(c->macro_args[0], &label) )    return complain();
                    
                                indent();
                                Serial.print(F("Keyboard writing \'"));
                                Serial.print(label);
                                Serial.println('\'');
                                Keyboard.write(c->macro_args[0]);
                                delete[] label;
                            }
                            break;

                        default: 
                            return complain_type();  
                    } break; // End type

                default: 
                    return complain_count();  
            } break; // End count

        case KEYBOARD_SETTXDELAY:
        // ----------------
            switch(c->arg_count) {
                case 1:
                    if(c->arg_type[0] != IntArg)    return complain();

                    indent();
                    Serial.print(F("From now on, leaving "));
                    Serial.print(c->int_args[0]);
                    Serial.println(F("ms between keyboard updates"));
                    Keyboard.setTxDelay(c->int_args[0]);     

                    break;

                default: 
                    return complain_count();
            } break;  


        default:
        // -----
            Serial.println(F("Error - Command unimplemented"));
            break;
    } // End of command switch

    // Blank line after every command
    Serial.println();
}

#endif