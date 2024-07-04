/*
 * File: xm23p.h
 * Description: Header file for the XM-23p Emulator. Defines constants, 
 * data structures, and function prototypes used across the emulator.
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */

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
#define BITS_PER_REGISTER 16
#define NUMBER_OF_REGISTERS 8
#define HEX_CHARS_PER_REGISTER 4
#define SET 1
#define CLEAR 0
#define WORD 0
#define BYTE 1
#define TRUE 1
#define FALSE 0


/* Time Counter Initialization */
//extern int timecounter; // time counter

/* initialize IMEM and DMEM (64kib each or 1<<16) */
unsigned char IMEM[MEMORY_SIZE];
unsigned char DMEM[MEMORY_SIZE];

/* Starting addresses buffer */
long int I_Start_Addresses;

/*Main functions (in xm23p.c)*/
void InitializeCPU(); // initializes CPU
void PrintMenuOptions(); // prints menu options

/*loader functions*/
void ProcessSRecords(char line[MAX_S_RECORD_SIZE]); // prcoesses line from xme file
void Send2IMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount); // sends the data to IMEM
void Send2DMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount); // sends the data to DMEM
void PrintMEM(); // prints both memory spaces
void PrintIMEM(); // prints IMEM
void PrintDMEM(); // prints DMEM

//int timecount=0; // time counter

/* Declaring the Registers*/
unsigned char RegistersBinaryString[NUMBER_OF_REGISTERS][BITS_PER_REGISTER]; // 8 registers each has 16 bits
unsigned char RegistersHexString[NUMBER_OF_REGISTERS][HEX_CHARS_PER_REGISTER]; // 8 registers each has 4 hex digits
unsigned int RegistersValue[NUMBER_OF_REGISTERS]; // 8 registers each has 32 bits
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define PC 7

/* PSW Register (vnzc) */
typedef struct { // psw register struct
    unsigned short v : SET; // overflow bit/flag
    unsigned short n : SET; // negative bit/flag
    unsigned short z : SET; // zero bit/flag
    unsigned short c : SET; // carry bit/flag
} PSW_Struct;
PSW_Struct PSW; // instance of PSW register

/* Clock */
extern unsigned short Timecount;

/* function to process instructions (fetch.c) */
void process_instruction(); // process one instruction
void fetch(); // fetches one instruction
char IMAR[HEX_CHARS_PER_REGISTER]; // store the instruction
long int E_Start_Addresses; // starting address for external/debug use

/* function to process instructions (decode.c) */
unsigned short IMARValue; // to convert for shifting
unsigned short rcbuff, wbbuff, srcbuff, dstbuff, // temporary operand buffers
    bitsbuff; 
unsigned short prpobuff, decbuff, incbuff, offbuff; // temporary operand buffers for DMEM functions
unsigned short vbuff, nbuff, zbuff, cbuff; // SETCC and CLRCC buffers
int decode(); // decodes one instruction
int betweenADDandST(unsigned short IMARValue); // ADD to ST instruction
int betweenMOVLandMOVH(unsigned short IMARValue); // MOVL to MOVH instruction
int betweenBEQandBRA(unsigned short IMARValue); // BEQ to BRA instruction

/* function to process instructions (execute.c) */
/* Opcode internal numbers for fast execution */
enum { 
    BL, BEQBZ, BNEBNZ, BCBHS, BNCBLO, BN, BGE, BLT, BRA, 
    ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, 
    BIT, BIC, BIS, MOV, SWAP, SRA, RRC, SWPB, SXT, 
    SETPRI, SVC, SETCC, CLRCC, CEX, LD, ST, MOVL, 
    MOVLZ, MOVLS, MOVH, LDR, STR, Error
}; // opcode cases
void execute(int instructionnumber); // executes one instruction

char currentInstructionBinary[BITS_PER_REGISTER]; // current instruction
int currentInstructionValue; // current instruction in decimal
char currentInstruction[HEX_CHARS_PER_REGISTER]; // current instruction in hex

/* debugger.c funcitons and variables */
extern unsigned short BreakpointValue; // breakpoint in decimal
void debug(); // main debug function
void UpdateRegistersBinary(char newcontent[HEX_CHARS_PER_REGISTER], int regnum); // update the registers binary content
void ChangedRegistersBinary(char newcontent[BITS_PER_REGISTER], int regnum); // change the registers binary content
void ChangedRegistersValue(unsigned short newcontent, int regnum); // change the registers value
void ChangedRegistersHexString(char newcontent[HEX_CHARS_PER_REGISTER], int regnum); // change the registers hex string
void process_instruction_debugger() // process all instruction in debug mode

//void UpdateRegistersValue(unsigned short newcontent, int regnum); // update the registers value
//char Breakpoint[4]; // breakpoint
//unsigned short opcodebuff;



#endif //XM23P_H