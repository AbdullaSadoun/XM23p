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
    //int count=0;
    fetch(); // fetches the instruction
    printf("IMARValue:%04X @PC=%04X \n", IMARValue, RegistersValue[PC]); // debug printf

    while(!(IMARValue == 0x0000 || RegistersValue[PC] == BreakpointValue)) { //|| count >= 50

        fetch();
        printf("IMARValue:%04X @PC=%04X\t", IMARValue, RegistersValue[PC]); // debug printf
        RegistersValue[PC] += 2;
        ChangedRegistersValue(RegistersValue[PC], PC);


        timecounter++;

        int instructionnumber = decode();
        timecounter++;

        execute(instructionnumber); // executes current instruction. 
        fetch(); // fetches next instruction
        timecounter++;
    } 

    printf("Breakpoint reached or the end of exec.\n");
}

void fetch(){ // fetch function
/*
function to fetch the instruction
- it fetches the instruction from memory
- it returns the fetched instruction
*/
    IMARValue = IMEM[RegistersValue[PC]/2]; // because IMEM is unsigned short

    return;
}

void step(){
/*
function to step through the instructions
- it fetches the instruction
- it decodes the instruction
- it executes the instruction
- it increments the time counter
*/
    fetch();

    if(IMARValue == 0x0000 || RegistersValue[PC] == BreakpointValue){
        printf("Breakpoint reached or the end of exec.\n");
        return;
    } else {
        fetch();
        
        RegistersValue[PC] += 2;
        ChangedRegistersValue(RegistersValue[PC], PC);

        int instructionnumber = decode();

        execute(instructionnumber); // executes current instruction. 
        return;
    }
    
}