/*
 * File: debugger.c
 * Description: Main debugging module for the XM-23p Emulator. Provides 
 * debugging functionalities such as setting breakpoints, viewing register 
 * contents, and stepping through instructions.
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"
#include <stdio.h>

unsigned short BreakpointValue;
unsigned short timecounter; // time counter

int buffer;
unsigned short D0 = 0; // Initialize D0
unsigned short E0 = 0; // Initialize E0
unsigned short F0;
unsigned short F1;
int instructionnumber;

void ChangedRegistersValue(unsigned short newcontent, int regnum){ 
/*
function to sync all the register forms when its value is changed
- updates the registers hex string
- changes the registers binary string
*/  

    // convert from new content's value to a Hex-string of characters
    sprintf(RegistersHexString[regnum], "%04X", newcontent);

    // convert the register binary to hex string
    for (int i = 15; i >= 0; --i, newcontent >>= 1) {
        RegistersBinaryString[regnum][i] = (newcontent & 1) + '0';
    }
    
    return;
}

void view_registers(){
/*
function to print the registers and PSW content
- prints the registers in binary and hex
- prints the PSW content
*/
    printf("Registers:\n");
        for(int i = 0; i < 8; i++){
            printf("R%d: ", i);
            printf("0b");
            for(int j = 0; j < 16; j++){
                printf("%c", RegistersBinaryString[i][j]);
            }
            printf(" | 0x%04X", RegistersValue[i]);
            
            printf("\n");
        }
        // print PSW content
        printf("PSW(vnzc): %d%d%d%d\n", PSW.v, PSW.n, PSW.z, PSW.c);
        return;
}

void edit_registers(){
/*
function to edit the registers content
- user is prompted to enter the register number and the new content
*/
    printf("Reg# (word in hex)= ");
    int regnum;
    scanf("%d %hx", &regnum, &RegistersValue[regnum]); // change the register content
    ChangedRegistersValue(RegistersValue[regnum], regnum); // update the register forms
    return;
}

void memory_printer(){
/*
function to print the memory content
- user is prompted to select the memory to print
- prints the memory content by calling PrintMEM
*/
    char memchoice;
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
    return;
}

void edit_memory(){
/*
function to edit the memory content
- user is prompted to select the memory to edit
- user is prompted to enter the address and the new content
*/
    char memchoice;
    printf("select Memory I=IMEM D=DMEM\n");
    scanf(" %c", &memchoice);

    if(memchoice == 'I' || memchoice == 'i'){ // edit IMEM
        unsigned short address, content;
        printf("Enter(address content): %hx %hx\n", address, content);
        IMEM[address/2] = content;
    } else if(memchoice == 'D' || memchoice == 'd'){ // edit DMEM
        unsigned short address, content;
        printf("Enter(address content): %hx %hx\n", address, content);
        DMEM[address/2] = content;
    } else { // invalid choice
        printf("Invalid choice\n");
    }
    return;
}

void run_debugger(){
/*
function to process the instruction in debugger mode
- it fetches the instruction
- it decodes the instruction
- it executes the instruction and fetches next one
- it increments the time counter
- it prints out what process has occured in the right sequence
*/

    printf("Clock\tPC\tInstruction\tFetch\t\tDecode\tExecute\n");
    int firstrun = TRUE;
    //unsigned short D0 = 0; // Initialize D0
    //unsigned short E0 = 0; // Initialize E0
    fetch(); // just to stop IMAR from being 0

    while(!(IMARValue == 0x0000 || RegistersValue[PC] == BreakpointValue)){
        
        fetch();
        // unsigned short F0 = RegistersValue[PC]; // Initialize F0
        // unsigned short F1 = IMARValue; // Initialize F1
        F0 = RegistersValue[PC]; // Initialize F0
        F1 = IMARValue; // Initialize F1

        int instructionnumber = decode(); // doesnt need pc

        RegistersValue[PC] += 2;
        ChangedRegistersValue(RegistersValue[PC], PC);

        printf("%d\t%04X\t%04X\t\tF0:%04X\t\tD0:%04X\n", timecounter, RegistersValue[PC], IMARValue, F0, D0);

        timecounter++;

        printf("%d\t\t \t\tF1:%04X\t\t\tE0:%04X \t", timecounter, F1, E0);

        if(firstrun == TRUE){
            buffer = instructionnumber;
            printf("\n");
            fetch(); // fetches next instruction F1
            D0 = F1;
            E0 = D0;
            firstrun = FALSE;
            timecounter++;
        } else {
            if (((TC == 0) && (FC == 0)) || buffer == CEX) { // CEX disabled
                execute(buffer); // executes current instruction. (CEX off)
            } else { // CEX enabled
                cex_enabled(buffer); // handle executions accordingly
            }
            //execute(buffer); // executes current instruction. E0
            fetch(); // fetches next instruction F1
            D0 = F1;
            E0 = D0;
            buffer = instructionnumber;
            timecounter++;
        }
    }
}

void step_debugger(){
/*
function to step through the instructions in debugger mode
- user can step through the instructions
- it fetches the instruction
- it decodes the instruction
- it executes the instruction and fetches next one
*/

    printf("Press 's' to step through instructions, 'q' to quit\n");
    printf("Clock\tPC\tInstruction\tFetch\t\tDecode\tExecute\n");
    int firstrun = TRUE;
    //unsigned short D0 = 0; // Initialize D0
    //unsigned short E0 = 0; // Initialize E0
    fetch(); // just to stop IMAR from being 0
    char choice = 's';

    while(!(IMARValue == 0x0000 || RegistersValue[PC] == BreakpointValue || choice == 'q')){
        
        fetch();
        // unsigned short F0 = RegistersValue[PC]; // Initialize F0
        // unsigned short F1 = IMARValue; // Initialize F1
        F0 = RegistersValue[PC]; // Initialize F0
        F1 = IMARValue; // Initialize F1

        int instructionnumber = decode(); // doesnt need pc

        RegistersValue[PC] += 2;
        ChangedRegistersValue(RegistersValue[PC], PC);

        printf("%d\t%04X\t%04X\t\tF0:%04X\t\tD0:%04X   ", timecounter, RegistersValue[PC], IMARValue, F0, D0);
        scanf(" %c", &choice);
        if(choice == 'q'){
            return;
        }

        timecounter++;

        printf("%d\t\t \t\tF1:%04X\t\t\tE0:%04X\t      ", timecounter, F1, E0);
        scanf(" %c", &choice);
        if(choice == 'q'){
            return;
        }

        if(firstrun == TRUE){
            buffer = instructionnumber;
            //printf("\n");
            fetch(); // fetches next instruction F1
            D0 = F1;
            E0 = D0;
            firstrun = FALSE;
            timecounter++;
        } else {
            if (((TC == 0) && (FC == 0)) || buffer == CEX) { // CEX disabled
                execute(buffer); // executes current instruction. (CEX off)
            } else { // CEX enabled
                cex_enabled(buffer); // handle executions accordingly
            }
            //execute(buffer); // executes current instruction. E0
            fetch(); // fetches next instruction F1
            D0 = F1;
            E0 = D0;
            buffer = instructionnumber;
            timecounter++;
        }
    }
    return;
}

void add_breakpoint(){
/*
function to add a breakpoint
- user is prompted to enter the breakpoint value
- prints the breakpoint value for verification
*/
    printf("Enter breakpoint value: ");
    scanf("%hx", &BreakpointValue);
    printf("Breakpoint set at: %04X\n", BreakpointValue);
    return;
}

void print_time(){
/*
function to print the time counter
*/
    printf("Time Count: %d\n", timecounter);
    return;
}