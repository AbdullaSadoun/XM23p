#include "loader.h"

void ProcessSRecords(char line[MAX_S_RECORD_SIZE]){
    // check if the line is an s-record
    if (line[0] != 'S' && line[0] != 's'){
        printf("Not an s-record\n");
        return;
    }

    // check the record type
    char type = line[1];
    char length[2];
    length[0] = line[2];
    length[1] = line[3];
    int length = strtol(length, NULL, 16);
    printf("Record length: %d\n", length);

    char address[4];
    char data[30];

    void Send2IMEM();
    void Send2DMEM();

    // type specific actions
    if(type == '0'){
        // print ascii data
        for(int i = 0; i < length; i++){
            printf("%c", line[2*i+4]);
        }
        
    } else if (type == '1'){
        // process s1 record
        // get the address
        char addr[4];
        addr[0] = line[4];
        addr[1] = line[5];
        addr[2] = line[2];
        addr[3] = line[3];
        //int address = strtol(addr, NULL, 16);
        // print address for debugging purposes
        printf("address s1: %c%c%c%c", addr[0], addr[1], addr[2], addr[3]);

        // get the data
        for(int i = 0; i < length; i++){
            data[i] = line[2*i+6];
            printf("%c", data[i]); // for debugging purposes
        }
    
        // int d = strtol(data, NULL, 16);
        // store the data in IMEM
        //IMEM[address] = d;

    } else if (type == '2'){ // process s2 record
        // get the address
        char addr[4];
        addr[0] = line[4];
        addr[1] = line[5];
        addr[2] = line[2];
        addr[3] = line[3];
        int address = strtol(addr, NULL, 16);

        // get the data
        
    } else if (type == '9'){
        // do nothing for now
    } else {
        printf("Unknown or Invalid record type\n");
    } 

    return;
}
void Send2IMEM();
void Send2DMEM();
void PrintMEM();