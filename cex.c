/*
 * File: cex.c
 * Description: Handles the conditional execution of instructions in the XM-23p Emulator.
 * 
 * 
 * Author: Abdulla Sadoun
 * 
 * - Date: 04/08/2024 
 * 
 */
#include "xm23p.h"

void cex(){
/*
function to handle conditional execution
- it sets the cex_condition based on the condition prefix
- it sets the TC and FC values to know number of instructions to execute
*/
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
}

void cex_enabled(int instructionnumber) {
/*
* function to handle the CEX effect on the next instructions
* - it will execute the instruction if the CEX condition is TRUE
* - it will skip the instruction if the CEX condition is FALSE
* - it will decrement the True counter if the instruction is executed
* - it will decrement the False counter if the instruction is skipped
*/
    if (cex_condition == TRUE) { // cex in effect, true should execute
        if(TC > 0){ // check if there are any trues left
            execute(instructionnumber); // executes trues
            TC--; // Decrement True counter
        } else { // TC = 0 (execute all trues)
            printf("CEX skip\n"); // skip the falses
            FC--; // Decrement False counter
        }
    } else { // cex_condition = FALSE, false should execute
        if(TC > 0){ // check if there are any trues left
            printf("CEX skip\n"); // skip the trues
            TC--; // Decrement True counter
        } else { // TC = 0 and FC>0(executed all trues) falses left
        execute(instructionnumber); // executes falses
        FC--; // Decrement False counter
        }
    }
    return;
}