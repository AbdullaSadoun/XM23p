#ifndef xm23p_h
#define xm23p_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1<<16 // 64kib
#define MAX_S_RECORD_SIZE 68 // 34 byte pairs
#define MAX_DATA_SIZE 60 // 30 byte pairs
#define MAX_ADDRESS_SIZE 4 // 2 byte pairs
#define BITS_PER_ROW 32 // 32 bits per row 

/*other constants used */
#define FIRST_ELEMENT_INDEX 0 
#define NEXT_ELEMENT_INDEX 1
#define THIRD_ELEMENT_INDEX 2
#define FOURTH_ELEMENT_INDEX 3
#define DATA_START_OFFSET 8
#define DATA_END_OFFSET 9
#define TWO_BITS_OFFSET 2

/* Time Counter Initialization */
//extern int timecounter; // time counter

/* initialize IMEM and DMEM (64kib each or 1<<16) */
unsigned char IMEM[MEMORY_SIZE];
unsigned char DMEM[MEMORY_SIZE];

/* Starting addresses buffer */
long int I_Start_Addresses;

/*functions*/
void ProcessSRecords(char line[MAX_S_RECORD_SIZE]); // prcoesses line from xme file
void Send2IMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount); // sends the data to IMEM
void Send2DMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount); // sends the data to DMEM
void PrintMEM(); // prints both memory spaces
void PrintIMEM(); // prints IMEM
void PrintDMEM(); // prints DMEM

//int timecount=0; // time counter

/* Declaring the Registers*/
unsigned char RegistersBinaryString[8][16]; // 8 registers each has 16 bits
// same register in hex
unsigned char RegistersHexString[8][4]; // 8 registers each has 4 hex digits
//initialize all registers to 0
//void InitializeRegisters();
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define PC 7

// function to process instructions
void process_instruction(); // process one instruction
void fetch(); // fetches one instruction
void decode(); // decodes one instruction
void execute(); // executes one instruction
void betweenADDandST(unsigned short IMARValue); // ADD to ST instruction
void betweenMOVLandMOVH(unsigned short IMARValue); // MOVL to MOVH instruction

char currentInstructionBinary[16]; // current instruction
int currentInstructionValue; // current instruction in decimal
char currentInstruction[4]; // current instruction in hex

// debugger.c funcitons
void debug(); // debug function
char Breakpoint[4]; // breakpoint
int BreakpointValue; // breakpoint in decimal
void UpdateRegistersBinary(char newcontent[4], int regnum); // update the registers binary content

long int E_Start_Addresses; // starting address for external/debug use

#endif //XM23P_H