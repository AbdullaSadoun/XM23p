/*
* File:   MEMinstructions.c
* Description: File containing the functions to execute memory instructions
*
* Author: Abdulla Sadoun
*
* - Date: 04/08/2024
*
*/

#include "xm23p.h"

void ld(){
/*
function to execute the ld(load) instruction
- it calculates the effective address
- it checks if the instruction is pre or post increment or decrement
- it checks if the instruction is word or byte
- it loads the word/byte from the memory to the destination register
*/
    if (prpobuff != POST) { // PRE-INC/DEC
        if (incbuff == SET) { // PRE-INC
            RegistersValue[srcbuff] += (wbbuff == WORD) ? 2 : 1;
        }
        if (decbuff == SET) { // PRE-DEC
            RegistersValue[srcbuff] -= (wbbuff == WORD) ? 2 : 1;
        }
        if (wbbuff == WORD) {
            RegistersValue[dstbuff] = DMEM[RegistersValue[srcbuff] / 2];
        } else {
            // Preserve the high byte of the destination register and only load the low byte from DMEM
            unsigned short byteVal;
            if (RegistersValue[srcbuff] % 2 == 0) {
                byteVal = DMEM[RegistersValue[srcbuff] / 2] & 0x00FF; // Low byte
            } else {
                byteVal = (DMEM[RegistersValue[srcbuff] / 2] & 0xFF00) >> 8; // High byte
            }
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | byteVal;
        }
    } else { // POST-INC/DEC
        if (wbbuff == WORD) {
            RegistersValue[dstbuff] = DMEM[RegistersValue[srcbuff] / 2];
        } else {
            // Preserve the high byte of the destination register and only load the low byte from DMEM
            unsigned short byteVal;
            if (RegistersValue[srcbuff] % 2 == 0) {
                byteVal = DMEM[RegistersValue[srcbuff] / 2] & 0x00FF; // Low byte
            } else {
                byteVal = (DMEM[RegistersValue[srcbuff] / 2] & 0xFF00) >> 8; // High byte
            }
            RegistersValue[dstbuff] = (RegistersValue[dstbuff] & 0xFF00) | byteVal;
        }

        if (incbuff == SET) { // POST-INC
            RegistersValue[srcbuff] += (wbbuff == WORD) ? 2 : 1;
        }
        if (decbuff == SET) { // POST-DEC
            RegistersValue[srcbuff] -= (wbbuff == WORD) ? 2 : 1;
        }
    }
    return;
}

void st(){
/*
function to execute the st(store) instruction
- it calculates the effective address
- it checks if the instruction is pre or post increment or decrement
- it checks if the instruction is word or byte
- it stores the word/byte from the source register to the memory
*/
    if (prpobuff != POST) { // PRE-INC/DEC
        if (incbuff == SET) { // PRE-INC
            RegistersValue[dstbuff] += (wbbuff == WORD) ? 2 : 1;
        }
        if (decbuff == SET) { // PRE-DEC
            RegistersValue[dstbuff] -= (wbbuff == WORD) ? 2 : 1;
        }
        if (wbbuff == WORD) {
            DMEM[RegistersValue[dstbuff] / 2] = RegistersValue[srcbuff]; // Store word
        } else {
            unsigned short wordVal = DMEM[RegistersValue[dstbuff] / 2];
            if (RegistersValue[dstbuff] % 2 == 0) {
                // Store to low byte
                wordVal = (wordVal & 0xFF00) | (RegistersValue[srcbuff] & 0x00FF);
            } else {
                // Store to high byte
                wordVal = (wordVal & 0x00FF) | ((RegistersValue[srcbuff] & 0x00FF) << 8);
            }
            DMEM[RegistersValue[dstbuff] / 2] = wordVal;
        }
    } else { // POST-INC/DEC
        if (wbbuff == WORD) {
            DMEM[RegistersValue[dstbuff] / 2] = RegistersValue[srcbuff]; // Store word
        } else {
            unsigned short wordVal = DMEM[RegistersValue[dstbuff] / 2];
            if (RegistersValue[dstbuff] % 2 == 0) {
                // Store to low byte
                wordVal = (wordVal & 0xFF00) | (RegistersValue[srcbuff] & 0x00FF);
            } else {
                // Store to high byte
                wordVal = (wordVal & 0x00FF) | ((RegistersValue[srcbuff] & 0x00FF) << 8);
            }
            DMEM[RegistersValue[dstbuff] / 2] = wordVal;
        }            
        if (incbuff == SET) { // POST-INC
            RegistersValue[dstbuff] += (wbbuff == WORD) ? 2 : 1;
        }
        if (decbuff == SET) { // POST-DEC
            RegistersValue[dstbuff] -= (wbbuff == WORD) ? 2 : 1;
        }
    }
    return;
}

void ldr(){
/*
function to execute the ldr instruction
- it calculates the effective address
- it loads the word or byte from the memory to the destination register
*/
    EA = RegistersValue[srcbuff] + (offsetbuff/2); // get the Effective address
    //RegistersValue[srcbuff] += offsetbuff; // get the Effective address
    if(wbbuff == WORD){
        RegistersValue[dstbuff] = DMEM[EA/2]; // dst shouldnt change
    } else { // BYTE
        RegistersValue[dstbuff] = DMEM[EA/2] & 0x00FF; // dst shouldnt change
    }
}

void str(){
/*
function to execute the str instruction
- it calculates the effective address
- it stores the word or byte from the source register to the memory
*/
    EA = RegistersValue[dstbuff] + offsetbuff; // get the Effective address
    if(wbbuff == WORD){
        DMEM[EA/2] = RegistersValue[srcbuff]; // store the word
    } else { // BYTE
        DMEM[EA/2] = (RegistersValue[srcbuff] & 0x00FF); // dst shouldnt change
    }
    return;
}