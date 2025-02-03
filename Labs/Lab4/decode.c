/*
 * File: decode.c
 * Description: Module responsible for decoding instructions in the XM-23p Emulator. 
 * It extracts opcode and operands from instructions and gives them an internal exec. number.
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"

unsigned short BreakpointValue; // breakpoint value (global variable)

#define LTCASE 0x7
#define STCASE 0x6
#define SUB_LD 0x0
#define MOV_CLRCC 0x3
#define CEX 0x4

int decode(){ // decode function
/*
function to decode the instruction
- it decodes the instruction based on the opcode
- it returns an number indicating the internal instruction
number for the execute function
- it decodes in layers to distinguish and seperate functions
*/
    enum { BLCase, BEQtoBRA, ADDtoST, MOVLtoMOVH, LLDR, LSTR}; // grouped into shared first 3 bits

    if((E_Start_Addresses-2) == BreakpointValue){ // extra breakpoint check
        printf("Breakpoint reached3\n");
        return Error;
    }
     if((RegistersValue[PC]) == BreakpointValue){ // extra breakpoint check
        printf("Breakpoint reached4\n");
        return Error;
    }

    //printf("decoded@%x instruction:%x ", (int) RegistersValue[PC]-2, IMARValue); // debugger printf
    unsigned short opcode = ((IMARValue>>13)&0x07); // get first 3 bits of the instruction
    unsigned short off = ((IMARValue>>7)&0x7f); // get the offset bit (LDR-STR)
    
    switch (opcode) { // first 3 bits cases
    case BLCase:
        return BL;
    case BEQtoBRA:
        return betweenBEQandBRA(IMARValue); // go to further filtering
    case ADDtoST:
        return betweenADDandST(IMARValue); // go to further filtering
    case MOVLtoMOVH:
        return betweenMOVLandMOVH(IMARValue); // go to further filtering
    case LLDR:
        return LDR; // LDR internal number sent to execute
    case LSTR:
        offbuff = off;
        return STR; // STR internal number sent to execute
    default:
        offbuff = off;
        printf("Error - instruction not yet implemented\n");
        return Error;
    }
}

int betweenADDandST(unsigned short IMARValue){ 
/*
decoding function for ADD to ST instructions
- it decodes the instruction based on the opcode
- it returns an number indicating the internal instruction
- it decodes functions between ADD and ST and sets their
buffers for the execute function
- will do further decoding (Layer2 and Layer3) if necessary
- L2 refers to Layer 2
- L3 refers to Layer 3
*/
    unsigned short opcode = ((IMARValue>>10)&0x07); // get L2 opcode
    prpobuff = ((IMARValue>>9)&0x01);  // get the PRPO bits
    decbuff = ((IMARValue>>8)&0x01); // get the decrement bit
    incbuff = ((IMARValue>>7)&0x01); // get increment bit
    wbbuff = ((IMARValue>>6)&0x01); // get the word/byte bit
    srcbuff = ((IMARValue>>3)&0x07); // get the source register
    dstbuff = ((IMARValue)&0x07); // get the destination register

    if (opcode == LTCASE || opcode == STCASE) { // LD or ST case
        unsigned short opcode = ((IMARValue>>10)&0x01); // L3 opcode
        prpobuff = ((IMARValue>>9)&0x01);  // get the PRPO bits
        decbuff = ((IMARValue>>8)&0x01); // get the decrement bit
        incbuff = ((IMARValue>>7)&0x01); // get increment bit
        wbbuff = ((IMARValue>>6)&0x01); // get the word/byte bit
        srcbuff = ((IMARValue>>3)&0x07); // get the source register
        dstbuff = ((IMARValue)&0x07); // get the destination register

        if (opcode == SUB_LD) { // LD case
            return LD;
        } else { // ST case
            return ST;
        }

    } else if (opcode == MOV_CLRCC) { // MOV to CLRCC case

        // get L3 opcode 
        //enum{LMOV, LSWAP, LSRA, LRRC, LSWPB, LSXT, LSETPRI, LSVC, LSETCC, LCLRCC}; // local opcode cases
        enum{LMOV, LSWAP, LSRALSXT, LSETPRILCLRCC, LSVC, LSETCC, LCLRCC}; // local opcode cases
        opcode = ((IMARValue>>7)&0x07);

        // set buffers for SETCC and CLRCC
        vbuff = ((IMARValue>>4)&0x01); // get the overflow bit
        nbuff = ((IMARValue>>2)&0x01); // get the negative bit
        zbuff = ((IMARValue>>1)&0x01); // get the zero bit
        cbuff = (IMARValue&0x01); // get the carry bit

        switch (opcode) { // L3 opcode cases
        case LMOV:
            return MOV;
        case LSWAP:
            return SWAP;
        case LSRALSXT:
            opcode = ((IMARValue>>3)&0x07); // get the L4 opcode
            enum{LSRA, LRRC, LSWPB, LSXT}; // local L4 opcode cases
            switch (opcode) {// L4 opcode cases
            case LSRA:
                return SRA;
            case LRRC:
                return RRC;
            case LSWPB:
                return SWPB;
            case LSXT:
                return SXT;
            default:
                printf("instruction not yet implemented\n");
                return Error;
            }
        case LSETPRILCLRCC:
            opcode = (IMARValue >> 4) & 0xf;
            if (opcode == 8){
                return SETPRI;
            } else if (opcode == 9){
                return SVC;
            } else if (opcode == 10 || opcode == 11){
                return SETCC;
            } else if (opcode == 12 || opcode <= 13){
                return CLRCC;
            } else {
                printf("instruction not yet implemented\n");
                return Error;
            }
        default:    
            printf("instruction not yet implemented\n");
            break;
            return Error;
        }

    } else if (opcode == CEX) { // CEX case
        return CEX;

    } else { // ADD to BIS case
        
        rcbuff = ((IMARValue>>7)&0x01); // get the register constant bit
        wbbuff = ((IMARValue>>6)&0x01); // get the word/byte bit
        srcbuff = ((IMARValue>>3)&0x07); // get the source register
        dstbuff = ((IMARValue)&0x07); // get the destination register

        opcode = ((IMARValue>>8)&0x0F); // get the L3 opcode

        enum{LADD, LADDC, LSUB, LSUBC, // loacl L3 pcode cases
            LDADD, LCMP, LXOR, LAND, 
            LOR, LBIT, LBIC, LBIS}; 

        switch (opcode) { // opcode cases
        case LADD:
            return ADD;
        case LADDC:
            return ADDC;
        case LSUB:
            return SUB;
        case LSUBC:
            return SUBC;
        case LDADD:
            return DADD;
        case LCMP:
            return CMP;
        case LXOR:
            return XOR;
        case LAND:
            return AND;
        case LOR:
            return OR;
        case LBIT:
            return BIT;
        case LBIC:
            return BIC;
        case LBIS:
            return BIS;
        default:
            printf("instruction not yet implemented\n");
            return Error;
        }
    } 
}

int betweenMOVLandMOVH(unsigned short IMARValue){
/*
function to decode the instruction between MOVL and MOVH
- it decodes the instruction based on the opcode
- it returns an number indicating the internal instruction
- it decodes functions between MOVL and MOVH and sets their
buffers for the execute function
*/
    unsigned short opcode = ((IMARValue>>11)&0x03); // get the Layer2 opcode
    dstbuff = ((IMARValue)&0x07); // get the destination register
    bitsbuff = ((IMARValue>>3)&0xFF); // get the bits

    enum{LMOVL, LMOVLZ, LMOVLS, LMOVH}; // local L2 opcode cases

    switch (opcode) { // opcode cases
    case LMOVL:
        return MOVL;
    case LMOVLZ:
        return MOVLZ;
    case LMOVLS:
        return MOVLS;
    case LMOVH:
        return MOVH;
    default:
        printf("instruction not yet implemented\n");
        return Error;
    }
}

int betweenBEQandBRA(unsigned short IMARValue){
/*
function to decode the instruction between BEQ and BRA
- it decodes the instruction based on the opcode
- it returns an number indicating the internal instruction
- it decodes functions between BEQ and BRA and sets their
buffers for the execute function
*/
    unsigned short opcode; // add a shift to get layer 2 opcode
    
    enum {LBEQBZ, LBNEBNZ, LBCBHS, // local L2 opcode cases
        LBNCBLO, LBN, LBGE, LBLT, LBRA};

    switch (opcode) { // opcode cases
    case LBEQBZ:
        return BEQBZ;
    case LBNEBNZ:
        return BNEBNZ;
    case LBCBHS:
        return BCBHS;
    case LBNCBLO:
        return BNCBLO;
    case LBN:
        return BN;
    case LBGE:
        return BGE;
    case LBLT:
        return BLT;
    case LBRA:
        return BRA;
    default:
        printf("instruction not yet implemented\n");
        return Error;
    }
}
