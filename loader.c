#include "loader.h"

void ProcessSRecords(char line[MAX_S_RECORD_SIZE]){
    if (line[0] != 'S' && line[0] != 's'){ // check if the line is an s-record
        printf("Not an s-record\n");
        return;
    }

    char TempBytePairStore[2]; // store byte pairs temporarily
    int checksum = 0; 

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
            printf("%c", strtol(TempBytePairStore, NULL, 16)); // print ascii 
            checksum += strtol(TempBytePairStore, NULL, 16); // increment checksum
        }

        unsigned int lsb = checksum & 0xFF; // check if checksum is valid (ends with FF)
        if (lsb == 0xFF) { // valid checksum
            printf(" Checksum is valid for s0\n");
        } else { // invalid checksum
            printf("Invalid Checksum");
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
            printf("Invalid Checksum");
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
            printf("Invalid Checksum");
        }

    } else if (type == '9'){ // process s9 record
        // check that its a vallid record

    } else { // invalid record type case
        printf("Unknown or Invalid record type\n");
    } 
    return;
}
void Send2IMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount){
    // store data starting from address in IMEM

    char conversionstring[4];
    conversionstring[0] = address[0];
    conversionstring[1] = address[1];
    conversionstring[2] = address[2];
    conversionstring[3] = address[3];
    
    int addr = strtol(conversionstring, NULL, 16); // get the address in decimal

    for(int i = 0; i <= dataloopcount; i+=2){  // store data in IMEM
        IMEM[addr+i] = data[i];
        IMEM[addr+i+1] = data[i+1];
        
    }

    return;
}
void Send2DMEM(char data[MAX_DATA_SIZE], char address[MAX_ADDRESS_SIZE], int dataloopcount){
   // store data starting from address in IMEM

    char conversionstring[4];
    conversionstring[0] = address[0];
    conversionstring[1] = address[1];
    conversionstring[2] = address[2];
    conversionstring[3] = address[3];
    
    int addr = strtol(conversionstring, NULL, 16); // get the address in decimal

    for(int i = 0; i <= dataloopcount; i+=2){ // store data in IMEM
        DMEM[addr+i] = data[i];
        DMEM[addr+i+1] = data[i+1];
    }
    return;
}
void PrintMEM(){

    int IMEMstart, IMEMend, DMEMstart, DMEMend;

    printf("Select IMEM range: ");
    scanf("%x %x", &IMEMstart, &IMEMend);

    printf("Select DMEM range: ");
    scanf("%x %x", &DMEMstart, &DMEMend);

    // print IMEM (16 byte pairs per row)
    printf("IMEM:\n"); // print IMEM label
    for(int i = IMEMstart; i < IMEMend; i+=32){
        printf("[%04x] ", i); // print address
        for(int j = 0; j < 32; j+=2){
            printf("%c%c ", IMEM[i+j], IMEM[i+j+1]);
        }
        printf("\n");
    }

    // print DMEM (16 byte pairs per row)
    printf("DMEM:\n"); // print DMEM label
    for(int i = DMEMstart; i < DMEMend; i+=32){
        printf("%04x: ", i); // print address
        for(int j = 0; j < 32; j+=2){
            printf("%c%c ", DMEM[i+j], DMEM[i+j+1]);
        }
        printf("\n");
    }

}
