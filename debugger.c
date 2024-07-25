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

void debug(){
    char choice2;

    printf("Debugging mode!\n");
    while(choice2 != 'q'){
        printf("===========DEBUGGER===========\n");
        printf("Choose an option: \n");
        printf("A - Run in debug mode\n");
        printf("R - View Registers Content\n");
        printf("E - Edit Register Content\n");
        printf("M - Display Memory\n"); 
        printf("I - Edit in IMEM\n");
        printf("D - Edit in DMEM\n");
        printf("B - Add Breakpoint\n");
        printf("S - Step (debugger UI unavailable in step)\n");
        printf("T - view Time Count\n");
        printf("Q - Quit\n");

        printf("Enter choice: ");
        scanf(" %c", &choice2);

        if(choice2 == 'R' || choice2 == 'r'){ // view registers content
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
        } else if(choice2 == 'e' || choice2 == 'E'){ // edit a registers content
            printf("Enter register number: ");
            int regnum;
            scanf("%d", &regnum);
            printf("Enter new content(hex): ");
            char newcontent[4];
            scanf("%s", newcontent);
            // change RegisterHexstring to new content 
            for(int i = 0; i < 4; i++){ // updating the register content
                RegistersHexString[regnum][i] = newcontent[i];
            }
            UpdateRegistersBinary(newcontent, regnum); // update the binary content
            printf("Register R%d content changed to %c%c%c%c\n", regnum, RegistersHexString[regnum][0], RegistersHexString[regnum][1], RegistersHexString[regnum][2], RegistersHexString[regnum][3]);

        } else if(choice2 == 'm' || choice2 == 'M'){ // display memory
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
        
        } else if(choice2 == 'i' || choice2 == 'I'){ // edit a value in IMEM
            unsigned short address, content;
            printf("Enter address (hex): ");
            scanf("%hx", &address);
            printf("Enter new content (hex): ");
            scanf("%hx", &content);
            IMEM[address/2] = content;

        } else if(choice2 == 'd' || choice2 == 'D'){ // edit in DMEM
            unsigned short address, content;
            printf("Enter address (hex): ");
            scanf("%hx", &address);
            printf("Enter new content (hex): ");
            scanf("%hx", &content);
            DMEM[address/2] = content;

        } else if(choice2 == 'b' || choice2 == 'B'){ // add a breakpoint
            printf("Breakpoint locaiton in Hex?: ");
            scanf("%hx", &BreakpointValue); // was %x
            printf("Breakpoint added at %x\n", BreakpointValue);

        } else if(choice2 == 's' || choice2 == 'S'){
            // step_debugger();
            step();

        } else if(choice2 == 't' || choice2 == 'T'){ // view the time counter value
            printf("Time Count: %d\n", timecounter);

        } else if (choice2 == 'a' || choice2 == 'A'){ // run in debug mode
            printf("Running in debug mode!!\n");
            process_instruction_debugger();
        
        } else if(choice2 == 'q' || choice2 == 'Q'){ // Quit
            return;

        } else {
            printf("Invalid choice, try again\n");
        } 
    }
}

void UpdateRegistersBinary(char newcontent[4], int regnum){
/*
function to update the registers binary content to display the new content
*/
    // get the int value of the new content which is in hex (char string)
    int newcontentvalue = strtol(newcontent, NULL, 16);

    // convert the new content (hex char string) to binary char string
    for (int i = 15; i >= 0; --i, newcontentvalue >>= 1) {
        RegistersBinaryString[regnum][i] = (newcontentvalue & 1) + '0';
    }

    return;
}

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

void ChangedRegistersHexString(char newcontent[4], int regnum){
/*
function to sync all the register forms when its Hex String is changed
- Updates the registers Hex value 
- Updates the registers binary string
*/
    // convert from Hex String to an int value
    int newcontentvalue = strtol(newcontent, NULL, 16);
    RegistersValue[regnum] = newcontentvalue;

    // convert the new content (hex char string) to binary char string
    for (int i = 15; i >= 0; --i, newcontentvalue >>= 1) {
        RegistersBinaryString[regnum][i] = (newcontentvalue & 1) + '0';
    }

    return;
}

void ChangedRegistersBinary(char newcontent[16] ,int regnum){
/*
function to sync all the register forms when its binary string is changed
- Updates the registers Hex value 
- Updates the registers Hex string
*/

    // convert the binary string to an int value
    int newcontentvalue = strtol(newcontent, NULL, 2);
    RegistersValue[regnum] = newcontentvalue;

    // convert the binary string to hex string
    sprintf(RegistersHexString[regnum], "%04X", newcontentvalue);

    return;
}

void process_instruction_debugger(){
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
            execute(buffer); // executes current instruction. E0
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
- it fetches the instruction
- it decodes the instruction
- it executes the instruction and fetches next one
*/
    printf("function is temporary unavailable\n");
    /*
    if(timecounter == 0){
        // initialize variables
        printf("Clock\tPC\tInstruction\tFetch\t\tDecode\tExecute\n");
 
        D0 = 0; // Initialize D0
        E0 = 0; // Initialize E0    
        buffer;

        //go through loop cycle 1
        fetch();
        I_Start_Addresses += 4;
        E_Start_Addresses += 2;
        RegistersValue[PC] = E_Start_Addresses;
        ChangedRegistersValue(RegistersValue[PC], PC);
        F0 = RegistersValue[PC]-2;
        F1 = IMARValue; // Initialize F1

        if (IMARValue == 0000 || RegistersValue[PC]-2 == BreakpointValue) {
            printf("Breakpoint reached or the end of exec.\n");
            return;
        }

        instructionnumber = decode(); // D0 runs with F0

        printf("%d\t%04X\t%04X\t\tF0:%04X\t\tD0:%04X\n", timecounter, RegistersValue[PC]-2, IMARValue, F0, D0);

        timecounter++;
    } else if(timecounter == 1){
        printf("%d\t\t \t\tF1:%04X\t\t\tE0:%04X \t", timecounter, F1, E0);

            buffer = instructionnumber;
            printf("\n");
            fetch(); // fetches next instruction F1
            D0 = F1;
            E0 = D0;
            timecounter++;
    } else if(timecounter % 2 == 0) { // F0 and D0
        // go through other loop cycles
        fetch();
        I_Start_Addresses += 4;
        E_Start_Addresses += 2;
        RegistersValue[PC] = E_Start_Addresses;
        ChangedRegistersValue(RegistersValue[PC], PC);
        F0 = RegistersValue[PC]-2;
        F1 = IMARValue; // Initialize F1

        if (IMARValue == 0000 || RegistersValue[PC]-2 == BreakpointValue) {
            printf("Breakpoint reached or the end of exec.\n");
            return;
        }
        int instructionnumber = decode(); // D0 runs with F0
        
        printf("%d\t%04X\t%04X\t\tF0:%04X\t\tD0:%04X\n", timecounter, RegistersValue[PC]-2, IMARValue, F0, D0);
        timecounter++;

    } else {
        // go through other loop cycles (1, 3 etc)
        printf("%d\t\t \t\tF1:%04X\t\t\tE0:%04X \t", timecounter, F1, E0);
        execute(buffer); // executes current instruction. E0
        fetch(); // fetches next instruction F1
        D0 = F1;
        E0 = D0;
        buffer = instructionnumber;
        timecounter++;
    }*/
    return;
}