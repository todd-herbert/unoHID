/* 
    Devkit
    -------------------------------------------
    File: parse.h

    - Struct which hold a parsed command
    - Function for parsing raw string input
*/

#ifndef __PARSE_H__
#define __PARSE_H__

#include <Arduino.h>
#include "unoHID.h"

#include "command_lookup.h"
#include "macro_lookup.h"

// How many arguments need to fit with one command
#define MAX_ARGS 3

// Struct which holds one serial command and its arguments
// -------------------------------------------------------
struct ParsedCommand {

    // Destructor - Free up dynamic memory
    ~ParsedCommand() {
               
        // Look for old string args
        for(uint8_t i = 0; i < arg_count; i++) {
            if (arg_type[i] == StringArg && string_args[i] != nullptr)
                delete[] string_args[i];
        }
    }    

    // Identifies the action to perform
    Command command;

    // How many arguments
    uint8_t arg_count = 0;

    // Type of each argument
    enum ArgType {
        StringArg,
        CharArg,
        IntArg,
        MacroArg,
    } arg_type[MAX_ARGS];

    // The argument data
    char *string_args[MAX_ARGS];
    int16_t int_args[MAX_ARGS];
    char char_args[MAX_ARGS];
    uint16_t macro_args[MAX_ARGS];

};

// Process char* from serial, into "ParsedCommand"
// returns true if more to process, false if finished
// --------------------------------------------------
bool parse(char*& serial_data, ParsedCommand* p) {

    // 1. Check for any more data, and setup
    // -----------------------------------------------------------------

    // Split at newline (replaced from semicolon) - multiple commands at once
    char *line;
    line = strtok_r(serial_data, "\n", &serial_data);

    // If all commands processed, return false;
    if (line == NULL)
        return false;

    // Re-initialize the ParsedCommand, if not empty
    if (p->arg_count != 0)
        *p = ParsedCommand();


    // 2. "command" portion: format, lookup, and store
    // -------------------------------------------------------------

    char *arg_splitter = line;
    char *arg;
    char* command_string = strtok_r(arg_splitter, "(", &arg_splitter);

    // Remove initial whitespace
    char *formatted = new char[strlen(command_string) + 1];
    uint8_t f = 0;
    uint8_t i = 0;
    while(i  < strlen(command_string) ) {
        switch (command_string[i]) {
            case ' ':
                i++;
                break;
            default:
                formatted[f] = command_string[i];
                f++;
                i++;
                break;
        }
        formatted[f] = '\0';
    }
    command_string = new char[strlen(formatted)];
    strcpy(command_string, formatted);
    delete[] formatted;
    
    // Find and store the ParsedCommand::Command value
    if (lookupCommand(command_string, &p->command))
        // (ParsedCommand::Command already stored by lookupCommand)
        delete[] command_string;
    else {
        Serial.println(F("Invalid Command"));
        Serial.println("");

        delete[] command_string;
        return false;   // Abort all commands
    }


    // 3. Split the "argument" portions, and handle 1 by 1
    // -----------------------------------------------------------------

    while (p->arg_count < MAX_ARGS)  {
        // Try split off another arg
        arg = strtok_r(arg_splitter, "\r", &arg_splitter);

        // If no more arguments, exit loop
        if (arg == NULL) break;

        // Grab some pointers, for code readability
        uint8_t *pc = &p->arg_count;                        // Arg count
        ParsedCommand::ArgType *pt = &p->arg_type[*pc];     // Arg types

        // Remove initial whitespace
        char *formatted = new char[strlen(arg) + 1];
        uint8_t f = 0;
        uint8_t i = 0;
        bool pre_string = true;
        while(i  < strlen(arg) ) {
            // If character is not initial space, copy
            if(arg[i] != ' ' || !pre_string) {
                formatted[f] = arg[i];
                f++;
            }
            // If character is start of string, allow spaces
            if (arg[i] == '\"')
                pre_string = false;
            i++;
        }
        formatted[f] = '\0';
        
        arg = new char[strlen(formatted)];
        strcpy(arg, formatted);
        delete[] formatted;

        // 3.1 Handle "no args"
        // ----------------------
        if (strcmp(arg, ")") == 0) {
            // All done with this command
            // Should re-run, check for another
            delete[] arg;
            return true;
        }

        // 3.2 Handle "char" args
        // ----------------------
        if (arg[0] == '\'') {
            // Mark this arg as char; store the parsed char value
            p->arg_type[p->arg_count] = ParsedCommand::ArgType::CharArg;
            p->char_args[*pc] = arg[1];
        }
        
        // 3.3 Handle "string" args
        // ------------------------
        else if (arg[0] == '\"') {
            // Mark this arg as string
            *pt = ParsedCommand::ArgType::StringArg;

            // Find the string boundary (inside double quotes)
            const uint8_t open = 0;
            uint8_t close;

            for(close = strlen(arg); close > open; close--) {
                if (arg[close] == '\"')
                    break;
            }

            // Allocate memory, in the ParsedCommand, for the string arg
            uint8_t length = (close - open) - 1;
            p->string_args[*pc] = (char*) new char[ length + 1 ];      // 1 extra for null terminator

            // Copy string arg into the ParsedCommand
            uint8_t s(open + 1), d(0);
            char* source = arg;
            char* dest = p->string_args[*pc];
            while (s < close) {
                dest[d] = source[s];
                s++;
                d++;
            }
            dest[d] = '\0';
        }

        // 3.4 Handle macro and int arguments
        // -----------------------------------------------------
        else {

            // Remove whitespace, and other inappropriate chars
            char *formatted = new char[strlen(arg)];
            uint8_t f = 0;
            uint8_t i = 0;
            while(i  < strlen(arg) ) {
                switch (arg[i]) {
                    case ',': case ' ': case ')':
                        i++;
                        break;
                    default:
                        formatted[f] = arg[i];
                        f++;
                        i++;
                        break;
                }
                formatted[f] = '\0';
            }
            arg = new char[strlen(formatted)+1];
            strcpy(arg, formatted);
            delete[] formatted;

            // 3.4.1 Check if argument is for the help command
            // -------------------------------------------------
            if (p->command == HELP) {
                // Store this macro as a string. Will parse in execute.h
                p->arg_type[0] = ParsedCommand::ArgType::StringArg;
                p->string_args[0] = new char[strlen(arg)];
                strcpy(p->string_args[0], arg);
            }

            // 3.4.2 Check if macro
            // ----------------------
            else if (lookupMacro(arg, &p->macro_args[*pc])) {
                // If arg was in table
                // ( already stored by lookupMacro )

                // Mark the argument as a macro
                *pt = ParsedCommand::ArgType::MacroArg;
            }

            // 3.4.3 Check if int
            // --------------------
            else {
                // Attempt conversion
                p->int_args[*pc] = atoi(arg);
                if (p->int_args[*pc] != 0 || (strcmp(arg, "0")) == 0 ) {
                    *pt = ParsedCommand::ArgType::IntArg;
                }
                // Unparsable argument, full abort.
                else {
                    Serial.println(F("Error - Argument not recognised as constant"));
                    Serial.println("");
                    delete[] arg;
                    return false; // Abort (remembering to delete arg)
                }
            }

            // Free resource - (string for splitting arguments)
            delete[] arg;
        }

        // 3.5 Iterate, loop and handle next arg
        // -------------------------------------
        p->arg_count++;
    }

    // 4. Finished one command
    // ------------------------
    // Should loop, looking for further commands in serial_data

    return true;
}


#endif