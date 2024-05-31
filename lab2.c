#include "xm23p.h"

// process instructions:
void process_instruction(){
    // convert the start address into binary to store in program counter
    
    // store the start address in program counter

    
    do { // keep running until 00 00 is encountered
        fetch();
        timecount++;

        decode();
        timecount++;

        execute();
        fetch();
        timecount++;
    } while(currentInstructionValue != 0000);
}


void fetch(){ // fetch function

    char IMAR[4]; // store the instruction

    // get the instruction from memory
    IMAR[0] = IMEM[I_Start_Addresses];
    IMAR[1] = IMEM[I_Start_Addresses+1];
    IMAR[2] = IMEM[I_Start_Addresses+2];
    IMAR[3] = IMEM[I_Start_Addresses+3];
    
    // increment the program counter (current address)
    I_Start_Addresses += 4;

    // return the fetched instruction
    printf("Instruction: %c%c\n", instruction[0], instruction[1]);
    // return fetched instruction;
    return;
}

/*
void decode(){ // decode function
    printf("decode\n");
    return 0;

}

void execute(){ // execute function
    printf("execute\n");
}
*/
