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

int timecount=0; // time counter

/* Declaring the Registers*/
unsigned char Registers[8][16]; // 8 registers each has 16 bits
#define R7 7
#define PC 7

// function to process instructions
void process_instruction(); // process one instruction
void fetch(); // fetches one instruction
void decode(); // decodes one instruction
void execute(); // executes one instruction

char currentInstructionBinary[16]; // current instruction
int currentInstructionValue; // current instruction in decimal
char currentInstruction[4]; // current instruction in hex

#endif //XM23P_H