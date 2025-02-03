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

    char choice; // char to temporarily store choice of the user

    // initializing CPU
    InitializeCPU(); // makes both mem.=0, all registers=0

    while(choice != 'q'){ // while user does not choose to quit
        
        PrintMenuOptions();

        scanf(" %c", &choice);
        //enum{LOAD, PRINT, FETCH, DEBUG, QUIT}; // menu options

        switch(choice) {
        case 'l': { // user chooses to load file
            char filename[150];
            printf("Enter filename: ");
            scanf("%s", filename);

            FILE *fin = fopen(filename, "r"); // opening input file
                if (fin == NULL) {
                    printf("File Open Error\n");
                    break;
                }

            // reading the file line by line and print lines read
            char line[MAX_S_RECORD_SIZE];
            while(fgets(line, MAX_S_RECORD_SIZE, fin) != NULL){
                ProcessSRecords(line); // process each s-record (line)
            }

            fclose(fin); // close the input file
            break;
        }
        case 'm': { // user chooses to print memory
            char memchoice;
            printf("select Memory I=IMEM D=DMEM B=both\n");
            scanf(" %c", &memchoice);

            if(memchoice == 'I' || memchoice == 'i'){ // print IMEM
                PrintIMEM();
            } else if(memchoice == 'D' || memchoice == 'd'){ // print DMEM
                PrintDMEM();
            } else if(memchoice == 'B' || memchoice == 'b'){ // print both
                PrintMEM();
            } else { // invalid choice
                printf("Invalid choice\n");
            }
            break;
        }
        case 'q': // user chooses to quit
            return 0;
        case 'f': // user chooses to fetch
            process_instruction(); 
            break;
        case 'd': // user chooses to debug
            debug(); // debug mode function
            break;
        default: // invalid choice
            printf("Invalid choice\n");
            break;
        }
    }
    
    return 0;
}