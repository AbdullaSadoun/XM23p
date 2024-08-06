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

/*
void cex_enabled(int instructionnumber) {
/*
* function to handle the CEX effect
* - it will execute the instruction if the CEX condition is TRUE
* - it will skip the instruction if the CEX condition is FALSE
* - it will decrement the True counter if the instruction is executed
* - it will decrement the False counter if the instruction is skipped
*
    if (cex_condition == TRUE) {
        if(TC > 0){ // check if there are any trues left
            execute(instructionnumber); // executes trues
            TC--; // Decrement True counter
        } else { // TC = 0 (execute all trues)
            printf("CEX skip\n"); // skip the falses
            FC--; // Decrement False counter
        }
    } else { // cex_condition = FALSE 
        if(TC > 0){ // check if there are any trues left
            printf("CEX skip\n"); // skip the trues
            TC--; // Decrement True counter
        } else { // TC = 0 and FC>0(executed all trues) falses left
        execute(instructionnumber); // executes falses
        FC--; // Decrement False counter
        }
    }
    return;
}
*/