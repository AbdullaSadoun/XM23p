/*
 * File: main.c
 * Description: Main file for XM-23p Emulator. Starts the CPU with all registers and memory initialized to 0. 
 *              Provides a user interface for the emulator with options to load files, print memory, execute instructions, debug, and quit.
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */

#include "xm23p.h"

int main(int argc, char* argv[]){

    char choice, memchoice, filename[MAX_FILENAME_LEN]; // char to temporarily store choice of the user

    // initializing CPU
    InitializeCPU(); // makes both mem.=0, all registers=0

    while(choice != 'q'){ // while user does not choose to quit
        
        PrintMenuOptions();

        scanf(" %c", &choice); // get user choice

        switch(choice) {
        case 'l':  // user chooses to load file
            printf("Enter filename: "); // prompt user for filename
            scanf("%s", filename); // get filename
            ProcessSRecords(filename); // loads and processes the s-records
            break;
        
        case 'v': // user chooses to view registers
            view_registers(); // prints registers
            break;

        case 'e': // user chooses to view registers
            view_registers(); // prints registers
            break;

        case 'm': // user chooses to print memory
            memory_printer(); // prints memory
            break;

        case 'c': // user chooses to change a word in memory
            edit_memory(); // edits memory 

        case 'r': // user chooses to run
            printf("starting address: %04X\n", RegistersValue[PC]);
            run(); // runs instructions from starting address
            break;

        case 's': // user chooses to step
            step(); // step through instructions
            break;

        case 'b': // user chooses to add a breakpoint
            add_breakpoint(); // adds a breakpoint
            break;
        
        case '1': // user chooses to run in debugging mode
            run_debugger(); // runs in debugging mode
            break;

        case '2': // user chooses to step in debugging mode
            step_debugger(); // steps in debugging mode
            break;

        case '3': // user chooses to view the time counter
            print_time(); // prints the time counter
            break;

        case 'q': // user chooses to quit
            return 0;
        
        default: // invalid choice
            printf("Invalid choice, try again!\n");
            break;
        }
    }
    return 0;
}