/*
 * File: run.c
 * Description: Handles the running of instructions in the XM-23p Emulator in REGULAR MODE(NODEBUG). 
 * Running the instruction refers to the fetching, decoding, and executing of the instruction. 
 * Will also handle special cases like CEX effect if enabled.
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 03/08/2024 
 * 
 */
#include "xm23p.h"

unsigned short timecounter; // time counter

void run(){
/*
function to process the instruction
- it fetches the instruction
- it decodes the instruction
- it executes the instruction
- it increments the time counter
*/
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

        if (((TC == 0) && (FC == 0)) || instructionnumber == CEX) { // CEX disabled
			execute(instructionnumber); // executes current instruction. (CEX off)
		} else { // CEX enabled
            cex_enabled(instructionnumber); // handle executions accordingly
        }

        //execute(instructionnumber); // executes current instruction. 
        fetch(); // fetches next instruction
        timecounter++;
    } 

    printf("Breakpoint reached or the end of exec.\n");
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
    printf("IMARValue:%04X @PC=%04X\t", IMARValue, RegistersValue[PC]); // debug printf

    if(IMARValue == 0x0000 || RegistersValue[PC] == BreakpointValue){
        printf("Breakpoint reached or the end of exec.\n");
        return;
    } else {
        fetch();
        
        RegistersValue[PC] += 2;
        ChangedRegistersValue(RegistersValue[PC], PC);

        int instructionnumber = decode();

         if (((TC == 0) && (FC == 0)) || instructionnumber == CEX) { // CEX disabled
			execute(instructionnumber); // executes current instruction. (CEX off)
		} else { // CEX enabled
            cex_enabled(instructionnumber); // handle executions accordingly
        }

        return;
    }
    
}

void step_x_times(){
/*
function to step through the instructions x times
- calls the step function x times
*/
    int x;
    printf("Enter the number of steps: ");
    scanf("%d", &x);

    for(int i = 0; i < x; i++){
        step();
    }
    return;
}