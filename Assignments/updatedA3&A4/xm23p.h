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

#define MEMORY_SIZE 65536 // 64kib
#define MAX_DATA_SIZE 60 // 30 byte pairs
#define MAX_ADDRESS_SIZE 4 // 2 byte pairs
#define BITS_PER_ROW 32 // 32 bits per row 
#define MAX_FILENAME_LEN 100 // max filename length
#define BYTES_PER_LINE 16
#define ASCII_START_VALUE 32 // SPEACE ASCII printable characters start at space
#define ASCII_END_VALUE 126 // ascii for ~ used for printable characters
#define HEADER_START 8 // Start of header in S-Record
#define BYTE_SIZE 2
#define ASCII_SIZE 4
#define MEM_SHIFT 1
#define BYTE_MASK 0xFF
#define DATA_SHIFT 8
#define MAX_S_RECORD_SIZE 100

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


/* Time Counter Initialization - Clock */
extern unsigned short Timecount;

/* initialize IMEM and DMEM (64kib each or 1<<16) */
//unsigned char IMEM[MEMORY_SIZE]; 
//unsigned char DMEM[MEMORY_SIZE];
unsigned short IMEM[MEMORY_SIZE/2];
unsigned short DMEM[MEMORY_SIZE/2];

/* Starting addresses buffer */
unsigned short I_Start_Addresses;

/*Main functions (in xm23p.c)*/
void InitializeCPU(); // initializes CPU
void PrintMenuOptions(); // prints menu options

/* loader.c functions */
void ProcessSRecords(const char* filename); // prcoesses line from xme file
void PrintMEM(unsigned char* MEM); // prints both memory spaces
unsigned int calculateChecksum(const char* line, int count, int dataLength); // calculates checksum

/* Declaring the Registers*/
unsigned char RegistersBinaryString[NUMBER_OF_REGISTERS][BITS_PER_REGISTER]; // 8 registers each has 16 bits
unsigned char RegistersHexString[NUMBER_OF_REGISTERS][HEX_CHARS_PER_REGISTER]; // 8 registers each has 4 hex digits
unsigned short RegistersValue[NUMBER_OF_REGISTERS]; // 8 registers each has 32 bits
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define LR 5
#define R6 6
#define SP 6
#define R7 7
#define PC 7

/* Frequentely accessed BITS */
#define BIT_NUMBER_0 0
#define BIT_NUMBER_6 6
#define BIT_NUMBER_9 9
#define BIT_NUMBER_12 12

/* PSW Register (vnzc) */
typedef struct { // psw register struct
    unsigned short v : SET; // overflow bit/flag
    unsigned short n : SET; // negative bit/flag
    unsigned short z : SET; // zero bit/flag
    unsigned short c : SET; // carry bit/flag
} PSW_Struct;
PSW_Struct PSW; // instance of PSW register

/* function to process instructions (fetch.c) */
void process_instruction(); // process one instruction
void fetch(); // fetches one instruction
void step(); // step function

/* function to process instructions (decode.c) */
unsigned short IMARValue; // to convert for shifting
unsigned short rcbuff, wbbuff, srcbuff, dstbuff, // temporary operand buffers
    bitsbuff;
short offsetbuff; 
unsigned short prpobuff, decbuff, incbuff, offbuff; // temporary operand buffers for DMEM functions
unsigned short vbuff, nbuff, zbuff, cbuff; // SETCC and CLRCC buffers
int decode(); // decodes one instruction
int BLdecode(); // function to decode BL instruction
int betweenADDandST(unsigned short IMARValue); // ADD to ST instruction
int betweenMOVLandMOVH(unsigned short IMARValue); // MOVL to MOVH instruction
int betweenBEQandBRA(unsigned short IMARValue); // BEQ to BRA instruction
int LDRdecode(); // function to decode LDR instruction
int STRdecode(); // function to decode STR instruction
short SignExt(short offset, int msb); // function to handle sign extensions

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
unsigned short EA; // effective address
unsigned short loadtoregister(unsigned short EA); // load from memory
void storeinmemory(unsigned short EA, unsigned short RegisterValue);

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
void process_instruction_debugger(); // process all instruction in debug mode
void step_debugger(); // step in debug mode


/* xm23p.c */
short SignExt(short offset, int msb);

#endif //XM23P_H