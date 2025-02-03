/*
* File:   BRANCHinstructions.c
* Description: File containing the functions to execute branch instructions
*
* Author: Abdulla Sadoun
*
* - Date: 04/08/2024
*/

#include "xm23p.h"

void bl(){
/*
function to execute the bl(branch and link) instruction
- it saves the return address in the link register
- it increments the PC by the offset
*/
    RegistersValue[LR] = RegistersValue[PC]; // make the link register equal to the PC
    RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    return;
}

void beqbz(){
/*
function to execute the beq/bz instruction
- it checks if the zero flag is set
- it increments the PC by the offset if the zero flag is set
*/
    if(PSW.z == TRUE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
    return;
}

void bnebnz(){
/*
function to execute the bne/bnz instruction
- it checks if the zero flag is cleared (=0)
- it increments the PC by the offset if the zero flag is not set
*/
    if(PSW.z == FALSE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    }
    return;
}

void bcbhs(){
/*
function to execute the bcb/hs instruction
- it checks if the carry flag is set
- it increments the PC by the offset if the carry flag is set
*/
    if(PSW.c == TRUE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
    return;
}

void bncblo(){
/*
function to execute the bnc/blo instruction
- it checks if the carry flag is not set
- it increments the PC by the offset if the carry flag is not set
*/
    if(PSW.c == FALSE){
            RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
        }
    return;
}

void bn(){
/*
function to execute the bn instruction
- it checks if the negative flag is set
- it increments the PC by the offset if the negative flag is set
*/
    if(PSW.n == TRUE){
        RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    }
    return;
}

void bge(){
/* 
function to execute the bge instruction
- it checks if the negative flag is equal to the overflow flag
- it increments the PC by the offset if the negative flag is equal to the overflow flag
*/
    if(PSW.n == PSW.v){
        RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    }
    return;
}

void blt(){
/*
function to execute the blt instruction
- it checks if the negative flag is not equal to the overflow flag
- it increments the PC by the offset if the negative flag is not equal to the overflow flag
*/
    if(PSW.n != PSW.v){
        RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    }
    return;
}

void bra(){
/*
function to execute the bra instruction
- it increments the PC by the offset always when called
*/
    RegistersValue[PC] = RegistersValue[PC] + offsetbuff; // increment the PC by offset
    return;
}
