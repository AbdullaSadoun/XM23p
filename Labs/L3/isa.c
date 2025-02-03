#include "xm23p.h"
char IMAR[4]; // store the instruction
unsigned short IMARValue; // to convert for shifting

// process instructions:
void process_instruction(){
    // convert the start address into binary to store in program counter
    
    // store the start address in program counter
    
    do { // keep running until 00 00 is encountered
        fetch();
        //timecounter++;

        decode();
        //timecounter++;

        //execute();
        //fetch();
        //timecounter++;
        
    } while(IMARValue != 0000 && E_Start_Addresses != BreakpointValue);
}


void fetch(){ // fetch function

    //char IMAR[4]; // store the instruction

    // get the instruction from memory
    IMAR[0] = IMEM[I_Start_Addresses];
    IMAR[1] = IMEM[I_Start_Addresses+1];
    IMAR[2] = IMEM[I_Start_Addresses+2];
    IMAR[3] = IMEM[I_Start_Addresses+3];
    
    // increment the program counter (current address)
    I_Start_Addresses += 4;
    E_Start_Addresses += 2;
    
    RegistersHexString[PC][0] = I_Start_Addresses >> 12;

    // return the fetched instruction
    //printf("Instruction: %c%c%c%c\n", IMAR[0], IMAR[1],IMAR[2],IMAR[3]); // debug printf

    // convert IMAR to unsigned short
    sscanf(IMAR, "%hx", &IMARValue);
    //printf("Instruction Value: %x\n", IMARValue); // check IMARValue
    // return fetched instruction;
    return;
}

enum { BLCase, BEQtoBRA, ADDtoST, MOVLtoMOVH, LDR, STR}; // grouped into shared first 3 bits

void decode(){ // decode function

    if((E_Start_Addresses-2) == BreakpointValue){
        printf("Breakpoint reached\n");
        return;
    }

    printf("decoded@%x instruction:%x ", (int) E_Start_Addresses-2, IMARValue); // debugger printf
    unsigned short opcode = ((IMARValue>>13)&0x07); // get first 3 bits of the instruction
    
    switch (opcode) { // first 3 bits cases
    case BLCase:
        printf("BL - tbd..\n"); // not yet implemented
        break;
    case BEQtoBRA:
        printf("BEQ-BRA - tbd..\n"); // not yet implemented
        break;
    case ADDtoST:
        //printf("instruction is between add and st\n"); // debug printf
        betweenADDandST(IMARValue); // go to further filtering
        break;
    case MOVLtoMOVH:
        //printf("instruction is between movl and movh\n"); // debug printf
        betweenMOVLandMOVH(IMARValue); // go to further filtering
        break;
    case LDR:
        printf("LDR - tbd..\n"); // not yet implemented
    case STR:
        printf("STR - tbd..\n"); // not yet implemented
        break;
    default:
        printf("instruction not yet implemented\n");
        break;
    }
    return;

}

void betweenADDandST(unsigned short IMARValue){ // decoding function for ADD to ST instructions
    unsigned short opcode = ((IMARValue>>10)&0x07); // get the sub opcode for L2 opcode

    // Different instruction handlers based on opcode

    if (opcode == 0x7 || opcode == 0x6) { // LD or ST case
        //printf("Layer2: LD or ST case\n"); // temporary debug printf
        unsigned short opcode = ((IMARValue>>10)&0x01); // L3 opcode
        unsigned short prpo = ((IMARValue>>9)&0x01);  // get the PRPO bits
        unsigned short dec = ((IMARValue>>8)&0x01); // get the decrement bit
        unsigned short inc = ((IMARValue>>7)&0x01); // get increment bit
        unsigned short wb = ((IMARValue>>6)&0x01); // get the word/byte bit
        unsigned short src = ((IMARValue>>3)&0x07); // get the source register
        unsigned short dst = ((IMARValue)&0x07); // get the destination register

        if (opcode == 0x0) { // LD case
            printf("LD: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpo, dec, inc, wb, src, dst); // debug printf 
        } else { // ST case
            printf("ST: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpo, dec, inc, wb, src, dst); // debug printf
        }

    } else if (opcode == 0x3) { // MOV to CLRCC case
        printf("Layer2: MOV_CLRCC case\n"); // temporary debug printf
        // get L3 opcode 
    } else if (opcode == 0x4) { // CEX case
        printf("Layer2: CEX case\n"); // temporary debug printf
    } else { // ADD to BIS case

        unsigned short rc = ((IMARValue>>7)&0x01); // get the register constant bit
        unsigned short wb = ((IMARValue>>6)&0x01); // get the word/byte bit
        unsigned short src = ((IMARValue>>3)&0x07); // get the source register
        unsigned short dst = ((IMARValue)&0x07); // get the destination register
        
        opcode = ((IMARValue>>8)&0x0F); // get the L3 opcode

        
        switch (opcode) { // opcode cases
        case 0x00:
            printf("ADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x01:
            printf("ADDC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x02:
            printf("SUB: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x03:
            printf("SUBC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x04:
            printf("DADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x05:
            printf("CMP: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x06:
            printf("XOR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x07:
            printf("AND: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x08:
            printf("OR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x09:
            printf("BIT: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x0A:
            printf("BIC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        case 0x0B:
            printf("BIS: RC=%d, WB=%d, SRC=%d, DST=%d\n", rc, wb, src, dst); // debug printf
            break;
        default:
            printf("instruction not yet implemented\n");
            break;
        }
    } 
    return;
}

void betweenMOVLandMOVH(unsigned short IMARValue){
    unsigned short opcode = ((IMARValue>>11)&0x03); // get the Layer2 opcode
    unsigned short dst = ((IMARValue)&0x07); // get the destination register
    unsigned short bits = ((IMARValue>>3)&0xFF); // get the bits

    switch (opcode) { // opcode cases
    case 0x00:
        printf("MOVL: dst:%d bits:%d\n", dst, bits);
        break;
    case 0x01:
        printf("MOVLZ: dst:%d bits:%d\n", dst, bits);
        break;
    case 0x02:
        printf("MOVLS: dst:%d bits:%d\n", dst, bits);
        break;
    case 0x03:
        printf("MOVH: dst:%d bits:%d\n", dst, bits);
        break;
    default:
        printf("instruction not yet implemented\n");
        break;
    }
    return;
}

/*
void execute(){ // execute function
    printf("execute\n");
}
*/
