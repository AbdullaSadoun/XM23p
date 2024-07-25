/*
 * File: execute.c
 * Description: Contains the execution logic for instructions in the XM-23p Emulator. It performs arithmetic and logical operations and updates the processor status word (PSW) accordingly.
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"

#define PRE 1
#define POST 0

#define WORD 0
#define BYTE 1

#define SET 1

void updatePSW(unsigned int src, unsigned int dst, unsigned int tempResult, int wbbuff){
/*
function to update the PSW flags
- it updates the PSW flags based on the result of the operation
- it updates the Zero (Z) flag
- it updates the Negative (N) flag
- it updates the Carry (C) flag
- it updates the Overflow (V) flag
*/
    // Clear the PSW flags
     PSW.z = CLEAR;
     PSW.n = CLEAR;
     PSW.c = CLEAR;
     PSW.v = CLEAR;// check if this should be cleared too
     
     // Check if the operation was on a byte or word
     int mask = (wbbuff == WORD) ? 0xFFFF : 0xFF;

    // Update the Zero (Z) flag
     if ((tempResult & mask) == CLEAR) {
          PSW.z = SET;
    }

     // Update the Negative (N) flag
    if ((tempResult & (mask + SET) >> SET) != CLEAR) {
        PSW.n = SET;
    }

    // Update the Carry (C) flag - In the context of CMP, carry flag is set if src < dst
    if (src < dst) {
        PSW.c = SET;
    }
    
}

void execute(int instructionnumber){ // execute function
/*
function to execute the instruction
- it executes the instruction based on the internal instruction number
- it updates the registers and PSW flags
- it updates the HexString and Binary files for the registers
- it prints out the execution details
*/
    unsigned short Temp_Destination; // temporary destination variable for swapping/replacing etc.

    switch(instructionnumber) { // opcode cases
    case BL:
        printf("BL: offset:%d\n", offsetbuff); // debug printf
        RegistersValue[LR] = RegistersValue[PC]; // make the link register equal to the PC
        RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        break;
    case BEQBZ: // Branch if equal
        printf("BEQ/BZ: offset:%d\n", offsetbuff); // debug printf
        if(PSW.z == TRUE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BNEBNZ: // Branch if not equal
        printf("BNE/BNZ: offset:%d\n", offsetbuff); // debug printf
        
        if(PSW.z == FALSE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BCBHS: // Branch if carry
        printf("BC/BHS: offset:%d\n", offsetbuff); // debug printf
        if(PSW.c == TRUE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BNCBLO:  // Branch if not carry
        printf("BNV/BLO: offset:%d\n", offsetbuff); // debug printf
        if(PSW.c == FALSE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BN: // Branch if negative
        printf("BN: offset:%d\n", offsetbuff); // debug printf
        if(PSW.n == TRUE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }

        break;
    case BGE: // Branch if greater than or equal
        printf("BGE: offset:%d\n", offsetbuff); // debug printf
        if(PSW.n == PSW.v){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BLT: // Branch if less than
        printf("BLT: offset:%d\n", offsetbuff); // debug printf
        if(PSW.n != PSW.v){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
        break;
    case BRA:
        printf("BRA: offset:%d\n", offsetbuff); // debug printf
        RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        break;
    case ADD: // DST = DST + SRC/CON
        printf("ADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf); // debug printf
        if(wbbuff == WORD){ // word addition
            RegistersValue[dstbuff] = RegistersValue[srcbuff] + RegistersValue[dstbuff]; // adding words
        } else { // byte addition
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte + dstLowByte; // adding bytes
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        // update PSW flags
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case ADDC: // DST = DST + (SRC/CON + Carry)
        printf("ADDC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf); // debug printf
        if(wbbuff == WORD){ // word addition
            RegistersValue[dstbuff] = RegistersValue[srcbuff] + RegistersValue[dstbuff] + PSW.c; // adding words and carry
        } else { // byte addition
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte + dstLowByte + PSW.c; // make sure to debug from here if anything goes wrong
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case SUB: // DST = DST + (-SRC/CON+1)
        printf("SUB: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if(wbbuff == WORD){ // word subtraction
            RegistersValue[dstbuff] = RegistersValue[dstbuff] + (~RegistersValue[srcbuff]+1);
        } else { // byte subtraction
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = dstLowByte + (srcLowByte + 1);
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case SUBC: // DST = DST + (-SRC/CON + Carry)
        printf("SUBC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if(wbbuff == WORD){ // word subtraction
             RegistersValue[dstbuff] = RegistersValue[dstbuff] + (~RegistersValue[srcbuff] + PSW.c);
        } else { // byte subtraction
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = dstLowByte + (srcLowByte + PSW.c);
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case DADD:
        printf("DADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if(wbbuff == WORD){ // word addition
            RegistersValue[dstbuff] = RegistersValue[srcbuff] + RegistersValue[dstbuff] + PSW.c; // adding words and carry
        } else { // byte addition
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte + dstLowByte + PSW.c; // make sure to debug from here if anything goes wrong
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case CMP:
        printf("CMP: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        //unsigned short Temp_Destination;
        if(wbbuff == WORD){ // word comparison
            Temp_Destination = RegistersValue[dstbuff] + ~RegistersValue[srcbuff] + 1;
        } else { // byte comparison
            unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            Temp_Destination = dstLowByte + ~srcLowByte + 1;
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], Temp_Destination, wbbuff);
        /*printf("R/C: %d\n" // Print the register/constant value
            "W/B: %d\n" // Print the word/byte value
            "Source: %hhx\n" // Print the value from the RegistersValue based on source
            "Destination: %hhx\n", // Print the value from the RegistersValue based on destination
        rcbuff, wbbuff, RegistersValue[srcbuff] & (wbbuff == WORD ? 0xFFFF : 0xFF), RegistersValue[dstbuff] & (wbbuff == WORD ? 0xFFFF : 0xFF));*/
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case XOR:
        printf("XOR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] ^= RegistersValue[srcbuff];
        } else { // If operation is byte-wide
            unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            unsigned short result = srcLowByte ^ dstLowByte; // XOR operation on the low bytes
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }

        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case AND:
        printf("AND: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] &= RegistersValue[srcbuff];
        } else { // If operation is byte-wide
            unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            unsigned short result = srcLowByte & dstLowByte; // AND operation on the low bytes
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case OR:
        printf("OR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] |= RegistersValue[srcbuff];
        } else { // If operation is byte-wide
            unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            unsigned short result = srcLowByte | dstLowByte; // OR operation on the low bytes
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case BIT:
        printf("BIT: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        //unsigned short Temp_Destination;
        if (wbbuff == WORD) { // If operation is word-wide
            Temp_Destination = RegistersValue[dstbuff] & (1 << RegistersValue[srcbuff]);
        } else { // If operation is byte-wide
            Temp_Destination = (RegistersValue[dstbuff] & 0x00FF) & (1 << (RegistersValue[srcbuff] & 0x00FF));
        }
        printf("R/C: %d\nW/B: %d\nSource: %hhx\nDestination: %hhx\n", 
            rcbuff, wbbuff, RegistersValue[srcbuff], RegistersValue[dstbuff]);
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], Temp_Destination, wbbuff);
        break;
    case BIC:
        printf("BIC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] &= ~(1 << RegistersValue[srcbuff]);
        } else { // If operation is byte-wide
            RegistersValue[dstbuff] &= ~(1 << (RegistersValue[srcbuff] & 0x00FF));
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case BIS: // DST = DST | (1 << SRC/CON)
        printf("BIS: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] |= (1 << RegistersValue[srcbuff]);
        } else { // If operation is byte-wide
            RegistersValue[dstbuff] |= (1 << (RegistersValue[srcbuff] & 0x00FF));
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case MOV:
        printf("MOV: WB=%d, SRC=%d, DST=%d\n", wbbuff, srcbuff, dstbuff); // debug printf (no need for r/c)
        if (wbbuff == WORD) { // If operation is word-wide
            RegistersValue[dstbuff] = RegistersValue[srcbuff];
        } else { // If operation is byte-wide
            // Clear the destination's low byte and set it to the source's low byte
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (RegistersValue[srcbuff] & 0x00FF);
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        break;
    case SWAP:
        printf("SWAP: SRC=%d, DST=%d\n", srcbuff, dstbuff); // debug printf
        unsigned short temp_reg = RegistersValue[dstbuff];
        RegistersValue[dstbuff] = RegistersValue[srcbuff];
        RegistersValue[srcbuff] = temp_reg;
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], 0, wbbuff); // Assuming PSW needs to be updated, adjust as necessary
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
        ChangedRegistersValue(RegistersValue[srcbuff], srcbuff);
        break;
    case SRA:
        printf("SRA: WB=%d, DST=%d\n", wbbuff, dstbuff);
        if (wbbuff == WORD) { // Word operation
            unsigned short msb = RegistersValue[dstbuff] & 0x8000; // Extract MSB (bit 15)
            RegistersValue[dstbuff] >>= 1; // Logical shift right by 1
            RegistersValue[dstbuff] |= msb; // Preserve MSB (sign bit for arithmetic shift)
        } else {
            // Byte operation
            unsigned char msb = RegistersValue[dstbuff] & 0x80; // Extract MSB (bit 7) of the low byte
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | ((RegistersValue[dstbuff] & 0x00FF) >> 1); // Logical shift right the low byte by 1
            RegistersValue[dstbuff] |= msb; // Preserve MSB (sign bit for arithmetic shift) in the low byte
        }
        //updatePSW(0, RegistersValue[dstbuff], 0, wbbuff); // Assuming PSW needs to be updated, adjust as necessary
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;
    case RRC:
        printf("RRC: WB=%d, DST=%d\n", wbbuff, dstbuff);
        unsigned short oldLSB;
        if (wbbuff == WORD) {
            // Word operation
            oldLSB = RegistersValue[dstbuff] & 0x0001; // Extract old LSB
            RegistersValue[dstbuff] >>= 1; // Shift right
            if (PSW.c) { // If carry is set
                RegistersValue[dstbuff] |= 0x8000; // Put carry into MSB
            }
            PSW.c = oldLSB; // Update carry with old LSB
        } else {
            // Byte operation
            oldLSB = RegistersValue[dstbuff] & 0x01; // Extract old LSB of the low byte
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | ((RegistersValue[dstbuff] & 0x00FF) >> 1); // Shift right the low byte
            if (PSW.c) { // If carry is set
                RegistersValue[dstbuff] |= 0x80; // Put carry into MSB of the low byte
            }
            PSW.c = oldLSB; // Update carry with old LSB
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;
    case SWPB:
        RegistersValue[dstbuff] = ((RegistersValue[dstbuff] & 0x00FF) << 8) | ((RegistersValue[dstbuff] & 0xFF00) >> 8); // Swap the high byte and low byte
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        printf("SWPB: DST=%d Result: %04X\n", dstbuff, RegistersValue[dstbuff]);
        //printf("Result: %04X\n", RegistersValue[dstbuff]);
        break;
    case SXT: // keep implementing until here.
        if (RegistersValue[dstbuff] & 0x0080) { // If bit 7 of the low byte is 1
            RegistersValue[dstbuff] |= 0xFF00; // Set bits 8-15 to 1 for sign extension
        } else {
            RegistersValue[dstbuff] &= 0x00FF; // Keep bits 8-15 as 0
        }
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        printf("SXT: DST=%d Result: %04X\n", dstbuff, RegistersValue[dstbuff]);
        break;
    case SETPRI:
        printf("SETPRI: \n");

        break;
    case SVC:
        printf("SVC: \n");

        break;
    case SETCC:
        printf("SETCC: \n");
        if (vbuff == TRUE) PSW.v = TRUE;
        if (nbuff == TRUE) PSW.n = TRUE;
        if (zbuff == TRUE) PSW.z = TRUE;
        if (cbuff == TRUE) PSW.c = TRUE;
        break;
    case CLRCC:
        printf("CLRCC: \n");
        if (vbuff == TRUE) PSW.v = FALSE;
        if (nbuff == TRUE) PSW.n = FALSE;
        if (zbuff == TRUE) PSW.z = FALSE;
        if (cbuff == TRUE) PSW.c = FALSE;
        break;
    case CEX:
        printf("CEX: \n");
        break;
    case LD:
        printf("LD: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpobuff, decbuff, incbuff, wbbuff, srcbuff, dstbuff); // debug printf
        EA = RegistersValue[srcbuff]; // Effective address in DMEM where data should be stored
        if (prpobuff != POST) { // PRE-INC/DEC
            if (incbuff == SET) { // PRE-INC
                EA += (wbbuff == WORD) ? 2 : 1;
            }
            if (decbuff == SET) { // PRE-DEC
                EA -= (wbbuff == WORD) ? 2 : 1;
            }
            // Load content from dmem into destination register
            RegistersValue[dstbuff] = DMEM[EA/2];
        } else { // POST-INC/DEC
            // Load content from dmem into destination register
            RegistersValue[dstbuff] = DMEM[EA/2];
            if (incbuff == SET) { // POST-INC
                EA += (wbbuff == WORD) ? 2 : 1;
            }
            if (decbuff == SET) { // POST-DEC
                EA -= (wbbuff == WORD) ? 2 : 1;
            }
        }
        break;
    case ST: // store content of source register to DMEM[EA(uses destination register)]
        printf("ST: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpobuff, decbuff, incbuff, wbbuff, srcbuff, dstbuff); // debug printf
        EA = RegistersValue[dstbuff]; // Effective address in DMEM where data should be stored
        if (prpobuff != POST) { // PRE-INC/DEC
            if (incbuff == SET) { // PRE-INC
                EA += (wbbuff == WORD) ? 2 : 1;
            }
            if (decbuff == SET) { // PRE-DEC
                EA -= (wbbuff == WORD) ? 2 : 1;
            }
            DMEM[EA/2] = RegistersValue[srcbuff];  // Store content from source register to dmem
        } else { // POST-INC/DEC
            DMEM[EA/2] = RegistersValue[srcbuff];  // Store content from source register to dmem
            if (incbuff == SET) { // POST-INC
                EA += (wbbuff == WORD) ? 2 : 1;
            }
            if (decbuff == SET) { // POST-DEC
                EA -= (wbbuff == WORD) ? 2 : 1;
            }
        }
        break;
    case MOVL:
        printf("MOVL: dst:%d bits:%d\n", dstbuff, bitsbuff); // debug printf
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | bitsbuff; // Set the low byte to bitsbuff, keep the high byte unchanged
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;
    case MOVLZ:
        printf("MOVLZ: dst:%d bits:%d\n", dstbuff, bitsbuff);
        RegistersValue[dstbuff] = bitsbuff; // Set the low byte to bitsbuff and high byte to 0
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;
    case MOVLS:
        printf("MOVLS: dst:%d bits:%d\n", dstbuff, bitsbuff);
        RegistersValue[dstbuff] = bitsbuff | 0xFF00; // Set the low byte to bitsbuff and high byte to 0xFF
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;
    case MOVH:
        printf("MOVH: dst:%d bits:%d\n", dstbuff, bitsbuff);
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0x00FF) | (bitsbuff << 8); // Set the high byte to bitsbuff, keep the low byte unchanged
        ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
        break;

    case LDR:
        printf("LDR: offset:%d wb:%d src:%d dst:%d\n", offsetbuff, wbbuff, srcbuff, dstbuff); // debug printf
        EA = RegistersValue[srcbuff] + offsetbuff; // get the Effective address
        if(wbbuff == WORD){
            RegistersValue[dstbuff] = DMEM[EA/2]; // Load the word from memory and increment it
        } else { // BYTE
            RegistersValue[dstbuff] = DMEM[EA/2] & 0x00FF; // Load the byte from memory and increment it
        }
        break;

    case STR:
        printf("STR: offset:%d wb:%d src:%d dst:%d\n", offsetbuff, wbbuff, srcbuff, dstbuff); // debug printf  
        EA = RegistersValue[dstbuff] + offsetbuff; // get the Effective address
        if(wbbuff == WORD){
            DMEM[EA/2] = RegistersValue[srcbuff]; // Store the word to memory and increment it
        } else { // BYTE
            DMEM[EA/2] = (RegistersValue[srcbuff] & 0x00FF); // Store the low byte to memory and increment it
        }
        break;

    case Error:
        printf("Error: \n");
        break;

    default:
        printf("instruction execution code not recognized\n");
        break;
    }
    //printf("@IMEM:%04X IMAR:%04X, \n", RegistersValue[PC], IMARValue); // print the PC and IMAR
    // update register binary content
    for(int i = 0; i < 8; i++ ){
        ChangedRegistersValue(RegistersValue[i], i);
    }
    return; 
}
