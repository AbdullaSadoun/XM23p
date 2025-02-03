#include "xm23p.h"

void ProcessSRecords(char line[MAX_S_RECORD_SIZE]){
/*
function checks if the records are valid
if they are valid then send2xmem would be called
to send them to appropriate memory space    
*/
    if (line[0] != 'S' && line[0] != 's'){ // check if the line is an s-record
        printf("Not an s-record\n");
        return;
    }

    char TempBytePairStore[2]; // store byte pairs temporarily to use strtol
    int checksum = 0; // initialize checksum

    char type = line[1]; // get the record type

    // get the length of the record
    char length[2];
    length[0] = line[2];
    length[1] = line[3];
    int lengthvalue = strtol(length, NULL, 16);

    checksum += lengthvalue; // add the length to the checksum

    char address[MAX_ADDRESS_SIZE]; // get the address
    address[0] = line[4];
    address[1] = line[5];
    address[2] = line[6];
    address[3] = line[7];
    TempBytePairStore[0] = address[0];
    TempBytePairStore[1] = address[1];
    TempBytePairStore[2] = '\0';
    checksum += strtol(TempBytePairStore, NULL, 16); // add address HI to checksum
   
    TempBytePairStore[0] = address[2];
    TempBytePairStore[1] = address[3];
    TempBytePairStore[2] = '\0';
    checksum += strtol(TempBytePairStore, NULL, 16); // add address LO to checksum

    char data[MAX_DATA_SIZE]; // get the data
    int dataloopcount = (lengthvalue-3)*2; // data loopcount limit
    // type specific actions
    if(type == '0'){ // process s0 record
        // print ascii data and calculate checksum
        for(int i = 0; i <= dataloopcount; i+=2){ // go through data and checksum
            TempBytePairStore[0] = line[i+8];
            TempBytePairStore[1] = line[i+9];
            TempBytePairStore[2] = '\0';
            printf("%c", (int) strtol(TempBytePairStore, NULL, 16)); // print ascii 
            checksum += strtol(TempBytePairStore, NULL, 16); // increment checksum
        }

        unsigned int lsb = checksum & 0xFF; // check if checksum is valid (ends with FF)
        if (lsb == 0xFF) { // valid checksum
            //printf(" Checksum is valid for s0\n");
            printf(" was loaded succefully\n");
        } else { // invalid checksum
            printf("Invalid Checksum for s0 record\n");
        }
        
    } else if (type == '1'){ // process s1 record

        for(int i = 0; i < dataloopcount+1; i+=2){ // get the data and checksum(+1)
            data[i] = line[i+8];
            data[i+1] = line[i+9];
            TempBytePairStore[0] = data[i]; // 
            TempBytePairStore[1] = data[i+1];
            TempBytePairStore[2] = '\0';
            checksum += strtol(TempBytePairStore, NULL, 16); // increment checksum
        }

        unsigned int lsb = checksum & 0xFF; // check if checksum is valid (ends with FF)
        if (lsb == 0xFF) { // valid checksum
            Send2IMEM(data, address,dataloopcount);
        } else { // invalid checksum
            printf("Invalid Checksum for s1 record\n");
        }

    } else if (type == '2'){ // process s2 record
        for(int i = 0; i < dataloopcount+1; i+=2){ // get the data and checksum(+1)
            data[i] = line[i+8];
            data[i+1] = line[i+9];
            TempBytePairStore[0] = data[i]; // 
            TempBytePairStore[1] = data[i+1];
            TempBytePairStore[2] = '\0';
            checksum += strtol(TempBytePairStore, NULL, 16); // increment checksum
        }

        unsigned int lsb = checksum & 0xFF; // check if checksum is valid (ends with FF)
        if (lsb == 0xFF) { // valid checksum
            Send2DMEM(data, address, dataloopcount);
        } else { // invalid checksum
            printf("Invalid Checksum for s2 record\n");
        }

    } else if (type == '9'){ // process s9 record
        // check that its a vallid record
        int i=0; // initialize i (loop counter)
        checksum = 0; // reset checksum

        while(line[i] != '\0' && line[i+1] != '\0' && line[i] != '\n' && line[i+1] != '\n'){
            TempBytePairStore[0] = line[i+2]; // 
            TempBytePairStore[1] = line[i+3];
            TempBytePairStore[2] = '\0';
            checksum += strtol(TempBytePairStore, NULL, 16); // increment checksum
            i+=2;
        }

    unsigned int lsb = checksum & 0xFF; // check if checksum is valid (ends with FF)
    if (lsb == 0xFF) { // valid checksum
        Send2DMEM(data, address, dataloopcount);
    } else { // invalid checksum
        printf("Invalid Checksum for s9 record\n");
    }

    } else { // invalid record type case
        printf("Unknown or Invalid record type\n");
    } 
    return;
}
void Send2IMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount){
/*
function is called to store data in IMEM
- converts address to integer 
-sets bits in IMEM to data bit starting from converted address
*/

    char conversionstring[4]; // string to use strtol
    conversionstring[0] = address[0];
    conversionstring[1] = address[1];
    conversionstring[2] = address[2];
    conversionstring[3] = address[3];
    
    int addr = strtol(conversionstring, NULL, 16); // get the address in decimal

    // save to the start address buffer for fetching later on
    I_Start_Addresses = addr;
    E_Start_Addresses = addr;

    int evenstate = 0; // 0 = even, 1 = odd
    for(int i = 0; i <= dataloopcount; i+=2){  // store data in IMEM
        if(evenstate == 0){
            IMEM[addr+i+2] = data[i]; // store data in IMEM
            IMEM[addr+i+1+2] = data[i+1]; // store data in IMEM
            evenstate = 1;
        } else {
            evenstate = 0;
            IMEM[addr+i-2] = data[i]; // store data in IMEM
            IMEM[addr+i+1-2] = data[i+1]; // store data in IMEM
        }

        //IMEM[addr+i] = data[i]; // store data in IMEM OG
        //IMEM[addr+i+1] = data[i+1]; // store data in IMEM OG
    
    }
    return;
}
void Send2DMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount){
/*
function is called to store data in DMEM
- converts address to integer 
-sets bits in DMEM to data bit starting from converted address
*/ 
   // store data starting from address in IMEM
    char conversionstring[4]; // string to use strtol
    conversionstring[0] = address[0];
    conversionstring[1] = address[1];
    conversionstring[2] = address[2];
    conversionstring[3] = address[3];
    
    int addr = strtol(conversionstring, NULL, 16); // get the address in decimal

    for(int i = 0; i <= dataloopcount; i+=2){ // store data in IMEM
        DMEM[addr+i] = data[i]; // store data in DMEM
        DMEM[addr+i+1] = data[i+1]; // store data in DMEM
    }
    return;
}
void PrintMEM(){
/*
function is called to print both IMEM and DMEM
- user is prompted to select range of memory to print
- IMEM and DMEM are printed in 16 byte pairs per row or 32 bits per row
*/
    int IMEMstart, IMEMend, DMEMstart, DMEMend;

    printf("Select IMEM range: ");
    scanf("%x %x", &IMEMstart, &IMEMend);

    printf("Select DMEM range: ");
    scanf("%x %x", &DMEMstart, &DMEMend);

    // print IMEM (16 byte pairs per row or 32 bits per row)
    printf("IMEM:\n"); // print IMEM label
    for(int i = IMEMstart; i < IMEMend; i+=BITS_PER_ROW){
        printf("%04x: ", i); // print address
        for(int j = 0; j < BITS_PER_ROW; j+=TWO_BITS_OFFSET){
            printf("%c%c ", IMEM[i+j], IMEM[i+j+NEXT_ELEMENT_INDEX]);
        }
        printf("\n");
    }

    // print DMEM (16 byte pairs per row or 32 bits per row)
    printf("DMEM:\n"); // print DMEM label
    for(int i = DMEMstart; i < DMEMend; i+=BITS_PER_ROW){
        printf("%04x: ", i); // print address
        for(int j = 0; j < BITS_PER_ROW; j+=TWO_BITS_OFFSET){
            printf("%c%c ", DMEM[i+j], DMEM[i+j+NEXT_ELEMENT_INDEX]);
        }
        printf("\n");
    }
}
void PrintIMEM(){
/*
function is called to print only IMEM
- user is prompted to select range of memory to print
- IMEM is printed in 16 byte pairs per row or 32 bits per row
*/
    int IMEMstart, IMEMend;

    printf("Select IMEM range: ");
    scanf("%x %x", &IMEMstart, &IMEMend);

    // print IMEM (16 byte pairs per row)
    printf("IMEM:\n"); // print IMEM label
    for(int i = IMEMstart; i < IMEMend; i+=BITS_PER_ROW){
        printf("%04x: ", i); // print address
        for(int j = 0; j < BITS_PER_ROW; j+=TWO_BITS_OFFSET){
            printf("%c%c ", IMEM[i+j], IMEM[i+j+NEXT_ELEMENT_INDEX]);
        }
        printf("\n");
    }
}
void PrintDMEM(){
/*
function is called to print only DMEM
- user is prompted to select range of memory to print
- DMEM is printed in 16 byte pairs per row or 32 bits per row
*/
    int DMEMstart, DMEMend;

    printf("Select IMEM range: ");
    scanf("%x %x", &DMEMstart, &DMEMend);

    // print IMEM (16 byte pairs per row)
    printf("IMEM:\n"); // print IMEM label
    for(int i = DMEMstart; i < DMEMend; i+=BITS_PER_ROW){
        printf("%04x: ", i); // print address
        for(int j = 0; j < BITS_PER_ROW; j+=TWO_BITS_OFFSET){
            printf("%c%c ", DMEM[i+j], DMEM[i+j+NEXT_ELEMENT_INDEX]);
        }
        printf("\n");
    }
}
