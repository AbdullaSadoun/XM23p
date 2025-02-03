/*
 * File: fetch.c
 * Description: Handles the fetching of instructions in the XM-23p Emulator. It updates the program counter and manages the instruction and execution start addresses.
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"

unsigned short timecounter; // time counter

void process_instruction(){
/*
function to process the instruction
- it fetches the instruction
- it decodes the instruction
- it executes the instruction
- it increments the time counter
*/

    while(TRUE){

        fetch();
        I_Start_Addresses += 4;
        E_Start_Addresses += 2;
        RegistersValue[PC] = E_Start_Addresses;
        ChangedRegistersValue(RegistersValue[PC], PC);
        timecounter++;

        if (IMARValue == 0000 || RegistersValue[PC]-2 == BreakpointValue) {// || (E_Start_Addresses-2) == BreakpointValue){
            printf("Breakpoint reached or the end of exec.\n");
            break;
        }

        int instructionnumber = decode();
        timecounter++;

        execute(instructionnumber); // executes current instruction. 
        fetch(); // fetches next instruction
        timecounter++;

    }
}

void fetch(){ // fetch function
/*
function to fetch the instruction
- it fetches the instruction from memory
- it returns the fetched instruction
*/
    //char IMAR[4]; // store the instruction

    // get the instruction from memory
    IMAR[0] = IMEM[I_Start_Addresses];
    IMAR[1] = IMEM[I_Start_Addresses+1];
    IMAR[2] = IMEM[I_Start_Addresses+2];
    IMAR[3] = IMEM[I_Start_Addresses+3];
    
    sscanf(IMAR, "%hx", &IMARValue);  // convert IMAR to unsigned short

    return;
}