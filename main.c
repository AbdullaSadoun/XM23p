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
        
        case 'm': // user chooses to print memory
            printf("select Memory I=IMEM D=DMEM B=both\n");
            scanf(" %c", &memchoice);

            if(memchoice == 'I' || memchoice == 'i'){ // print IMEM
                PrintMEM((unsigned char*)IMEM); // prints instruction memory
            } else if(memchoice == 'D' || memchoice == 'd'){ // print DMEM
                PrintMEM((unsigned char*)DMEM); // prints data memory
            } else if(memchoice == 'B' || memchoice == 'b'){ // print both
                PrintMEM((unsigned char*)IMEM); // prints instruction memory
                PrintMEM((unsigned char*)DMEM); // prints data memory
            } else { // invalid choice
                printf("Invalid choice\n");
            }
            break;
    
        case 'q': // user chooses to quit
            return 0;

        case 'r': // user chooses to run
            printf("starting address: %04X\n", RegistersValue[PC]);
            process_instruction(); 
            break;
        
        case 's': // user chooses to step
            step(); // step function
            break;

        case 'd': // user chooses to debug
            debug(); // debug mode function
            break;

        default: // invalid choice
            printf("Invalid choice, try again!\n");
            break;
        }
    }
    
    return 0;
}