/*
* File:   REGinstructions.c
* Description: File containing the functions to execute register instructions
*
* Author: Abdulla Sadoun
*
* - Date: 04/08/2024
*
*/

#include "xm23p.h"

void add(){ // add instruction
/*
function to execute the add instruction
- it checks if the instruction is word or byte
- it adds the source register to the destination register
- it updates the PSW flags
*/
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
    return;
}

void addc(){
/*
function to execute the addc instruction
- it checks if the instruction is word or byte
- it adds the source register to the destination register and the carry
- it updates the PSW flags
*/
    if(wbbuff == WORD){ // word addition
        RegistersValue[dstbuff] = RegistersValue[srcbuff] + RegistersValue[dstbuff] + PSW.c; // adding words and carry
    } else { // byte addition
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte + dstLowByte + PSW.c; // make sure to debug from here if anything goes wrong
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    return;
}

void sub(){
/*
function to execute the sub instruction
- it checks if the instruction is word or byte
- it subtracts the source register from the destination register
- it updates the PSW flags
*/
    if(wbbuff == WORD){ // word subtraction
        RegistersValue[dstbuff] = RegistersValue[dstbuff] + (~RegistersValue[srcbuff]+1);
    } else { // byte subtraction
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = dstLowByte + (srcLowByte + 1);
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    return;
}

void subc(){
/*
function to execute the subc instruction
- it checks if the instruction is word or byte
- it subtracts the source register from the destination register and the carry
- it updates the PSW flags
*/
    if(wbbuff == WORD){ // word subtraction
         RegistersValue[dstbuff] = RegistersValue[dstbuff] + (~RegistersValue[srcbuff] + PSW.c);
    } else { // byte subtraction
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = dstLowByte + (srcLowByte + PSW.c);
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    return;
}

void dadd(){
/*
function to execute the dadd instruction
- it checks if the instruction is word or byte
- it adds the source register to the destination register
- it updates the PSW flags if needed
*/
    if(wbbuff == WORD){ // word addition
        RegistersValue[dstbuff] = RegistersValue[srcbuff] + RegistersValue[dstbuff] + PSW.c; // adding words and carry
    } else { // byte addition
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte + dstLowByte + PSW.c; // make sure to debug from here if anything goes wrong
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
    return;
}

void cmp(){
/*
function to execute the cmp instruction
- it checks if the instruction is word or byte
- it subtracts the source register from the destination register
- it updates the PSW flags
*/
    unsigned short Temp_Destination;
    if(rcbuff == SET){ // constant in src comparison
        unsigned short constantmask = handleConstant(srcbuff);
        if(wbbuff == WORD){ // word comparison
            Temp_Destination = RegistersValue[dstbuff] + ~constantmask + 1;
        } else { // byte comparison
            unsigned short srcLowByte = constantmask & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            Temp_Destination = dstLowByte + ~srcLowByte + 1;
        }
        updatePSW(constantmask, RegistersValue[dstbuff], Temp_Destination, wbbuff);

    } else { // register in src comparison
        if(wbbuff == WORD){ // word comparison
            Temp_Destination = RegistersValue[dstbuff] + ~RegistersValue[srcbuff] + 1;
        } else { // byte comparison
            unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
            unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
            Temp_Destination = dstLowByte + ~srcLowByte + 1;
        }
        updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], Temp_Destination, wbbuff);
    }
    return;
}

void xor(){
/*
function to execute the xor instruction
- it checks if the instruction is word or byte
- it performs the XOR operation between the source register and the destination register
- it updates the PSW flags
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] ^= RegistersValue[srcbuff];
    } else { // If operation is byte-wide
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte ^ dstLowByte; // XOR operation on the low bytes
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    return;
}

void and(){
/*
function to execute the and instruction
- it checks if the instruction is word or byte
- it performs the AND operation between the source register and the destination register
- it updates the PSW flags
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] &= RegistersValue[srcbuff];
    } else { // If operation is byte-wide
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte & dstLowByte; // AND operation on the low bytes
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    return;
}

void or(){
/*
function to execute the or instruction
- it checks if the instruction is word or byte
- it performs the OR operation between the source register and the destination register
- it updates the PSW flags
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] |= RegistersValue[srcbuff];
    } else { // If operation is byte-wide
        unsigned short srcLowByte = RegistersValue[srcbuff] & 0x00FF;
        unsigned short dstLowByte = RegistersValue[dstbuff] & 0x00FF;
        unsigned short result = srcLowByte | dstLowByte; // OR operation on the low bytes
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (result & 0x00FF);
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    return;
}

void bit(){
/*
function to execute the bit instruction
- it checks if the instruction is word or byte
- it performs the bit operation between the source register and the destination register
- it updates the PSW flags
*/
    
    unsigned short Temp_Destination;
    if (wbbuff == WORD) { // If operation is word-wide
        Temp_Destination = RegistersValue[dstbuff] & (1 << RegistersValue[srcbuff]);
    } else { // If operation is byte-wide
        Temp_Destination = (RegistersValue[dstbuff] & 0x00FF) & (1 << (RegistersValue[srcbuff] & 0x00FF));
    }
    printf("R/C: %d\nW/B: %d\nSource: %hhx\nDestination: %hhx\n", 
        rcbuff, wbbuff, RegistersValue[srcbuff], RegistersValue[dstbuff]);
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], Temp_Destination, wbbuff);
    return;
}

void bic(){
/*
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] &= ~(1 << RegistersValue[srcbuff]);
    } else { // If operation is byte-wide
        RegistersValue[dstbuff] &= ~(1 << (RegistersValue[srcbuff] & 0x00FF));
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
    return;
}

void bis(){
/*
function to execute the bis instruction
- it checks if the instruction is word or byte
- it performs the bis operation between the source register and the destination register
- it updates the PSW flags
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] |= (1 << RegistersValue[srcbuff]);
    } else { // If operation is byte-wide
        RegistersValue[dstbuff] |= (1 << (RegistersValue[srcbuff] & 0x00FF));
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
    return;
}

void mov(){
/*
function to execute the mov instruction
- it checks if the instruction is word or byte
- it moves the source register to the destination register
- it updates the PSW flags
*/
    if (wbbuff == WORD) { // If operation is word-wide
        RegistersValue[dstbuff] = RegistersValue[srcbuff];
    } else { // If operation is byte-wide
        // Clear the destination's low byte and set it to the source's low byte
        RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | (RegistersValue[srcbuff] & 0x00FF);
    }
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], RegistersValue[dstbuff], wbbuff);
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
    return;
}

void swap(){
/*
function to execute the swap instruction
- it swaps the source register with the destination register
- it updates the PSW flags
*/
    unsigned short temp_reg = RegistersValue[dstbuff];
    RegistersValue[dstbuff] = RegistersValue[srcbuff];
    RegistersValue[srcbuff] = temp_reg;
    updatePSW(RegistersValue[srcbuff], RegistersValue[dstbuff], 0, wbbuff); // Assuming PSW needs to be updated, adjust as necessary
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff);
    ChangedRegistersValue(RegistersValue[srcbuff], srcbuff);
    return;
}

void sra(){
/*
function to execute the sra instruction
- it checks if the instruction is word or byte
- it shifts the destination register right
*/
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
}

void rrc(){
/*
function to execute the rrc instruction
- it checks if the instruction is word or byte
- it rotates the destination register right
- it updates the PSW flags
*/
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
    return;
}

void swpb(){
/*
function to execute the swpb instruction
- it swaps the high byte with the low byte of the destination register
- it keeps the low byte unchanged
*/
    RegistersValue[dstbuff] = ((RegistersValue[dstbuff] & 0x00FF) << 8) | ((RegistersValue[dstbuff] & 0xFF00) >> 8); // Swap the high byte and low byte
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    printf("SWPB: DST=%d Result: %04X\n", dstbuff, RegistersValue[dstbuff]);
    //printf("Result: %04X\n", RegistersValue[dstbuff]);
    return;
}

void sxt(){
/*
function to execute the sxt instruction
- it sign extends the low byte of the destination register
- it keeps the high byte unchanged
*/
    if (RegistersValue[dstbuff] & 0x0080) { // If bit 7 of the low byte is 1
        RegistersValue[dstbuff] |= 0xFF00; // Set bits 8-15 to 1 for sign extension
    } else {
        RegistersValue[dstbuff] &= 0x00FF; // Keep bits 8-15 as 0
    }
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    return;
}

void setcc(){
/*
function to execute the setcc instruction
- it sets the PSW flags according to the buffers
*/
    if (vbuff == TRUE) PSW.v = TRUE;
    if (nbuff == TRUE) PSW.n = TRUE;
    if (zbuff == TRUE) PSW.z = TRUE;
    if (cbuff == TRUE) PSW.c = TRUE;
    return;
}

void clrcc(){
/*
function to execute the clrcc instruction
- it clears the PSW flags if needed
*/
    if (vbuff == TRUE) PSW.v = FALSE;
    if (nbuff == TRUE) PSW.n = FALSE;
    if (zbuff == TRUE) PSW.z = FALSE;
    if (cbuff == TRUE) PSW.c = FALSE;
    return;
}

void movl(){
/*
function to execute the movl instruction
- it sets the low byte of the destination register to the bitsbuff
- it keeps the high byte unchanged
*/
    RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | bitsbuff; // Set the low byte to bitsbuff, keep the high byte unchanged
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    return;
}

void movlz(){
/*
function to execute the movlz instruction
- it sets the low byte of the destination register to the bitsbuff
- it keeps the high byte unchanged
*/
    RegistersValue[dstbuff] = bitsbuff; // Set the low byte to bitsbuff and high byte to 0
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    return;
}

void movls(){
/*
function to execute the movls instruction
- it sets the low byte of the destination register to the bitsbuff
- it keeps the high byte unchanged
*/
    RegistersValue[dstbuff] = bitsbuff | 0xFF00; // Set the low byte to bitsbuff and high byte to 0xFF
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    return;
}

void movh(){
/*
function to execute the movh instruction
- it sets the high byte of the destination register to the bitsbuff
- it keeps the low byte unchanged
*/
    RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0x00FF) | (bitsbuff << 8); // Set the high byte to bitsbuff, keep the low byte unchanged
    ChangedRegistersValue(RegistersValue[dstbuff], dstbuff); // updates the register's HexString/binary files
    return;
}
