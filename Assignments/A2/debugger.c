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

unsigned short BreakpointValue;

void debug(){
    char choice2;

    printf("Debugging mode!\n");
    while(choice2 != 'q'){
        printf("===========DEBUGGER===========\n");
        printf("Choose an option: \n");
        printf("R - View Registers Content\n");
        printf("E - Edit Register Content\n");
        printf("M - Display Memory\n"); 
        printf("I - Edit in IMEM\n");
        printf("D - Edit in DMEM\n");
        printf("B - Add Breakpoint\n");
        printf("S - Step\n");
        printf("Q - Quit\n");

        printf("Enter choice: ");
        scanf(" %c", &choice2);

        if(choice2 == 'R' || choice2 == 'r'){ // view registers content
            printf("Registers:\n");
            for(int i = 0; i < 8; i++){

                printf("R%d: \n", i);
                printf("binary:");
                for(int j = 0; j < 16; j++){
                    printf("%c", RegistersBinaryString[i][j]);
                }
                printf("\nHex: ");
                for(int j = 0; j < 4; j++){
                    printf("%c", RegistersHexString[i][j]);
                }
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
                PrintIMEM();
            } else if(memchoice == 'D' || memchoice == 'd'){ // print DMEM
                PrintDMEM();
            } else if(memchoice == 'B' || memchoice == 'b'){ // print both
                PrintMEM();
            } else {
                printf("Invalid choice, try again\n");
            }
        
        } else if(choice2 == 'i' || choice2 == 'I'){ // edit a value in IMEM
            printf("Enter address(Hex): ");
            char addresschar[4];
            scanf("%s", addresschar);
            printf("Enter new content(hex): ");
            char newcontent[4];
            scanf("%s", newcontent);
            Send2IMEM(newcontent, addresschar, 2);

        } else if(choice2 == 'd' || choice2 == 'D'){ // edit in DMEM
            printf("Enter address(Hex): ");
            char addresschar[4];
            scanf("%s", addresschar);
            printf("Enter new content(hex): ");
            char newcontent[4];
            scanf("%s", newcontent);
            Send2DMEM(newcontent, addresschar, 2);

        } else if(choice2 == 'b' || choice2 == 'B'){ // add a breakpoint
            // breakpoint to stop program counter from incrementing
            /*printf("Breakpoint locaiton in Hex?: \n");
            scanf("%s", Breakpoint);
            BreakpointValue = strtol(Breakpoint, NULL, 16);
            printf("Breakpoint added at %x\n", BreakpointValue);*/
            //unsigned short BreakpointValue;
            printf("Breakpoint locaiton in Hex?: ");
            scanf("%x", &BreakpointValue);
            printf("Breakpoint added at %x\n", BreakpointValue);

        } else if(choice2 == 's' || choice2 == 'S'){
            fetch();
            I_Start_Addresses += 4;
            E_Start_Addresses += 2;
            RegistersValue[PC] = E_Start_Addresses;
            ChangedRegistersValue(RegistersValue[PC], PC);

            if (IMARValue == 0000 || RegistersValue[PC]-2 == BreakpointValue) {// || (E_Start_Addresses-2) == BreakpointValue){
                printf("Breakpoint reached or the end of exec.\n");
                break;
            }

            int instructionnumber = decode();
            execute(instructionnumber);

        } else if(choice2 == 'q' || choice2 == 'Q'){
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
