/*
 * File: execute.c
 * Description: Contains the execution logic for instructions in the XM-23p Emulator. It performs arithmetic and logical operations and updates the processor status word (PSW) accordingly.
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"


void execute(int instructionnumber){ // execute function
/*
function to execute the instruction
- it calls appropriate function to execute the instruction based on 
    the internal instruction number
- it updates the registers and PSW flags
- it updates the HexString and Binary files for the registers
- it prints out the execution details
*/
    unsigned short Temp_Destination, Temp_src_buffer; // temporary destination variable for swapping/replacing etc.

    if(instructionnumber >= ADD && instructionnumber <= BIS && rcbuff == SET){
        Temp_src_buffer = RegistersValue[srcbuff]; // set the source buffer to the temporary source buffer
        RegistersValue[srcbuff] = handleConstant(srcbuff); // handle the constant value
    }

    switch(instructionnumber) { // opcode cases
    case BL: // branch and link
        printf("BL: offset:%d\n", offsetbuff); // debug printf
        bl(); // 
        break;
        
    case BEQBZ: // Branch if equal
        printf("BEQ/BZ: offset:%d\n", offsetbuff); // debug printf
        beqbz();
        break;

    case BNEBNZ: // Branch if not equal
        printf("BNE/BNZ: offset:%d\n", offsetbuff); // debug printf
        bnebnz();
        break;

    case BCBHS: // Branch if carry
        printf("BC/BHS: offset:%d\n", offsetbuff); // debug printf
        bcbhs();
        break;

    case BNCBLO:  // Branch if not carry
        printf("BNV/BLO: offset:%d\n", offsetbuff); // debug printf
        bncblo();
        break;

    case BN: // Branch if negative
        printf("BN: offset:%d\n", offsetbuff); // debug printf
        bn();
        break;

    case BGE: // Branch if greater than or equal
        printf("BGE: offset:%d\n", offsetbuff); // debug printf
        bge();
        break;

    case BLT: // Branch if less than
        printf("BLT: offset:%d\n", offsetbuff); // debug printf
        blt();
        break;

    case BRA: // Branch always
        printf("BRA: offset:%d\n", offsetbuff); // debug printf
        bra();
        break;
    
    case ADD: // add registers
        printf("ADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf); // debug printf
        add(); //  DST = DST + SRC/CON
        break;

    case ADDC: // add registers with carry
        printf("ADDC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf); // debug printf
        addc(); // DST = DST + (SRC/CON + Carry)
        break;

    case SUB: // subtract registers
        printf("SUB: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        sub(); // DST = DST + (-SRC/CON + 1)
        break;

    case SUBC: // subtract registers with carry
        printf("SUBC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        subc(); // DST = DST + (-SRC/CON + 1 + Carry)
        break;

    case DADD: // add registers (decimals)
        printf("DADD: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        dadd(); // DST = DST + SRC/CON
        break;

    case CMP: // compare registers
        printf("CMP: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        cmp(); // DST - SRC/CON
        break;

    case XOR: // exclusive OR registers
        printf("XOR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        xor(); // DST = DST ^ SRC/CON
        break;

    case AND: // AND registers
        printf("AND: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        and(); // DST = DST & SRC/CON
        break;

    case OR: // OR registers
        printf("OR: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        or(); // DST = DST | SRC/CON
        break;

    case BIT: // BIT test
        printf("BIT: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        bit(); // DST & SRC/CON
        break;

    case BIC: // bit clear
        printf("BIC: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        bic(); // DST = DST & ~(SRC/CON)
        break;

    case BIS: // bit set
        printf("BIS: RC=%d, WB=%d, SRC=%d, DST=%d\n", rcbuff, wbbuff, srcbuff, dstbuff); // debug printf
        bis(); // DST = DST | (1 << SRC/CON)
        break;

    case MOV: // move registers
        printf("MOV: WB=%d, SRC=%d, DST=%d\n", wbbuff, srcbuff, dstbuff); // debug printf (no need for r/c)
        mov(); // DST = SRC/CON
        break;

    case SWAP: // swap registers
        printf("SWAP: SRC=%d, DST=%d\n", srcbuff, dstbuff); // debug printf
        swap(); // DST = SRC
        break;

    case SRA: // shift right arithmetic
        printf("SRA: WB=%d, DST=%d\n", wbbuff, dstbuff);
        sra(); // DST = DST >> 1
        break;

    case RRC: // rotate right through carry
        printf("RRC: WB=%d, DST=%d\n", wbbuff, dstbuff);
        rrc(); // DST = DST >> 1 + Carry
        break;

    case SWPB: // swap bytes
        printf("SWPB: DST=%d Result: %04X\n", dstbuff, RegistersValue[dstbuff]);
        swpb(); // DST = ((DST & 0xFF) << 8) | ((DST >> 8) & 0xFF)
        break;

    case SXT: // sign extend
        sxt(); // DST = (DST & 0x80) ? 0xFF00 | DST : DST
        printf("SXT: DST=%d Result: %04X\n", dstbuff, RegistersValue[dstbuff]);
        break;

    case SETPRI: // not yet implemented
        printf("SETPRI: \n");
        break;

    case SVC: // not yet implemented
        printf("SVC: \n");
        break;

    case SETCC: // set psw flags
        printf("SETCC: \n");
        setcc(); // set the PSW flags
        break;

    case CLRCC: // clear psw flags
        printf("CLRCC: \n");
        clrcc(); // clear the PSW flags
        break;

    case CEX: // conditional execution
        cex(); // set the cex condition
        printf("CEX: Cond:%04X TC:%d FC:%d ", condition_prefix_buff, tcountbuff, fcountbuff);
        printf("%s\n", cex_condition ? "TRUE" : "FALSE");
        break;

    case LD: // load content to register
        printf("LD: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpobuff, decbuff, incbuff, wbbuff, srcbuff, dstbuff); // debug printf
        ld(); // DST = DMEM[SRC plus addressing]
        break;

    case ST: // store content from register to DMEM
        printf("ST: PRPO:%d DEC:%d INC:%d WB:%d SRC:%d DST:%d\n", prpobuff, decbuff, incbuff, wbbuff, srcbuff, dstbuff); // debug printf
        st(); // DMEM[DST plus addressing] = SRC
        break;

    case MOVL: // move low bits
        printf("MOVL: dst:%d bits:%d\n", dstbuff, bitsbuff); // debug printf
        movl(); // DST = SRC/CON
        break;

    case MOVLZ: // move low bits and zero rest
        printf("MOVLZ: dst:%d bits:%d\n", dstbuff, bitsbuff);
        movlz(); // DST = SRC/CON
        break;

    case MOVLS: // move low bits and set high 
        printf("MOVLS: dst:%d bits:%d\n", dstbuff, bitsbuff);
        movls(); // DST = SRC/CON
        break;

    case MOVH: // move high bits
        printf("MOVH: dst:%d bits:%d\n", dstbuff, bitsbuff);
        movh(); // DST = SRC/CON
        break;

    case LDR: // load from memory to register with offset
        printf("LDR: offset:%d wb:%d src:%d dst:%d\n", offsetbuff, wbbuff, srcbuff, dstbuff); // debug printf
        ldr(); // DST = mem[SRC + sign-extended 7-bit offset]
        break;

    case STR: // store from register to memory with offset
        printf("STR: offset:%d wb:%d src:%d dst:%d\n", offsetbuff, wbbuff, srcbuff, dstbuff); // debug printf  
        str(); // mem[DST + sign-extended 7-bit offset] = SRC
        break;

    case Error: // error
        printf("Error: \n"); // return error message
        break;

    default:
        printf("instruction execution code not recognized\n");
        break;
    }

    if(instructionnumber >= ADD && instructionnumber <= BIS && rcbuff == SET){ // Constant is taken from the register
        RegistersValue[srcbuff] = Temp_src_buffer; // set the source buffer to the temporary source buffer after constant was used
    }

    for(int i = 0; i < 8; i++ ){ // update register binary content
        ChangedRegistersValue(RegistersValue[i], i);
    }
    
    return; 
}

unsigned short handleConstant(int src) {
/*
function to handle constant values
- works when RC bit is set
- returns the constant value
- for the appropriate register no. (src)
*/
    switch (src) {
        case R0: return 0;
        case R1: return 1;
        case R2: return 2;
        case R3: return 4;
        case R4: return 8;
        case R5: return 16;
        case R6: return 32;
        case R7: return -1;
        default:
            printf("Error: Invalid register/constant value\n");
            return 0;
    }
}

/*
void cex(){
/*
function to handle conditional execution
- it sets the cex_condition based on the condition prefix
- it sets the TC and FC values to know number of instructions to execute
*
    // enum for the cex conditions prefix cases
    enum{EQ, NE, CSHS, CCLO, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, TR, FL}; // enum 
    
    // inspect the psw based on condition prefix
    switch (condition_prefix_buff) {
        case EQ:  // Equal / Zero
            cex_condition = (PSW.z == SET) ? TRUE : FALSE;  // Z flag is set
            break;
        case NE:  // Not Equal
            cex_condition = (PSW.z == CLEAR) ? TRUE : FALSE;  // Z flag is clear
            break;
        case CSHS:  // Carry Set / Unsigned Higher or Same
            cex_condition = (PSW.c == SET) ? TRUE : FALSE;  // C flag is set
            break;
        case CCLO:  // Carry Clear / Unsigned Lower
            cex_condition = (PSW.c == CLEAR) ? TRUE : FALSE;  // C flag is clear
            break;
        case MI:  // Minus / Negative
            cex_condition = (PSW.n == SET) ? TRUE : FALSE;  // N flag is set
            break;
        case PL:  // Plus / Positive or Zero
            cex_condition = (PSW.n == CLEAR) ? TRUE : FALSE;  // N flag is clear
            break;
        case VS:  // Overflow Set
            cex_condition = (PSW.v == SET) ? TRUE : FALSE;  // V flag is set
            break;
        case VC:  // No Overflow
            cex_condition = (PSW.v == CLEAR) ? TRUE : FALSE;  // V flag is clear
            break;
        case HI:  // Unsigned Higher
            cex_condition = (PSW.c == SET && PSW.z == CLEAR) ? TRUE : FALSE;  // C set and Z clear
            break;
        case LS:  // Unsigned Lower or Same
            cex_condition = (PSW.c == CLEAR || PSW.z == SET) ? TRUE : FALSE;  // C clear or Z set
            break;
        case GE:  // Greater or Equal (Signed)
            cex_condition = (PSW.n == PSW.v) ? TRUE : FALSE;  // N equal to V
            break;
        case LT:  // Less Than (Signed)
            cex_condition = (PSW.n != PSW.v) ? TRUE : FALSE;  // N not equal to V
            break;
        case GT:  // Greater Than (Signed)
            cex_condition = (PSW.z == CLEAR && PSW.n == PSW.v) ? TRUE : FALSE;  // Z clear and N equal to V
            break;
        case LE:  // Less Than or Equal (Signed)
            cex_condition = (PSW.z == SET || PSW.n != PSW.v) ? TRUE : FALSE;  // Z set or N not equal to V
            break;
        case TR:  // Always True
            cex_condition = TRUE;  // Always execute
            break;
        case FL:  // Always False
            cex_condition = FALSE;  // Never execute
            break;
        default:
            printf("Error: Invalid condition prefix\n");
            break;
    }

    TC = tcountbuff;
    FC = fcountbuff;
    return;
}*/


