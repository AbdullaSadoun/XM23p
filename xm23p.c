/*
 * File: xm23p.c
 * Description: Other functions used in the main.c implementation file for 
 * the XM-23p Emulator like nitializing the emulator, processeing user commands, 
 * and manageing the execution flow of instructions, as well as other functions 
 * that are often used within the emulator.
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */

#include "xm23p.h"

unsigned short timecounter; // time counter

void InitializeCPU(){
/*
function to initialize the CPU registers and memory
- it sets the IMEM to 0s
- it sets the DMEM to 0s
- it sets the RegistersBinaryString to 0s
- it sets the RegisterHexString to 0s
- it initializes the time counter to 0
*/

    // initialize IMEM and DMEM to be all zeros initially when booting
    /*for(int i = 0; i < MEMORY_SIZE/2; i++){
        IMEM[i] = 0;
        DMEM[i] = 0;
    }*/

    memset(IMEM, 0, sizeof(IMEM)); // initializing IMEM to 0s
    memset(DMEM, 0, sizeof(DMEM)); // initializing DMEM to 0s

    

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 16; j++){
            RegistersBinaryString[i][j] = '0';
        }
        for(int j = 0; j < 4; j++){
            RegistersHexString[i][j] = '0';
        }
    }

    timecounter = 0; // initializing time counter to 0

}

void PrintMenuOptions(){
/*
function to print out the main menu options 
*/
    printf("===========MENU===========\n");
    printf("l - load file\n");
    printf("v - view registers\n");
    printf("e - edit register values(word)\n");
    printf("m - display memory\n");
    printf("c - change in memory\n");
    printf("r - run (normal)\n");
    printf("s - step (normal)\n");
    printf("b - breakpoint\n");
    printf("x - step x times\n");
    printf("1- run (debug)\n");
    printf("2- step (debug)\n");
    printf("3- view time count\n");
    printf("q - quit\n");

}
