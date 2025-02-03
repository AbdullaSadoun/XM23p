/*
 * File: loader.c
 * Description: Responsible for loading S-Record files into the XM-23p Emulator's memory. Parses the S-Record format and updates IMEM and DMEM accordingly.
 * Author: Abdulla Sadoun
 * 
 * - Date: 26/06/2024 
 * 
 */
#include "xm23p.h"

void ProcessSRecords(const char* filename) {
/*
this function is called to process the s-records
- opens the file and reads the records
- checks if the records are valid
- if they are s0 record then its printed
- if they are s1 record then the data is stored in IMEM
- if they are s2 record then the data is stored in DMEM
- if they are s9 record then the starting address and PC is set
*/

    unsigned int byte;
    unsigned int data;

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    
    char line[MAX_S_RECORD_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != 'S') continue; // Not an S-Record
        int count, address;
        sscanf(line + 2, "%2x%4x", &count, &address); // Read count and address
        int dataLength = (count - 3) * BYTE_SIZE;

        unsigned int calculatedChecksum = calculateChecksum(line, count, dataLength); //check sum test
        unsigned int givenChecksum;
        sscanf(line + 2 + count * BYTE_SIZE, "%2x", &givenChecksum); // checksum done before storing

        if (calculatedChecksum != givenChecksum) {
            printf("Checksum error in line: %s\n", line);
            continue;
        }

        if (line[1] == '0') { // S0 record processing
            for (int i = 0; i < dataLength; i += BYTE_SIZE) {
                sscanf(line + HEADER_START + i, "%2x", &byte);
                printf("%c", byte);
            }
            printf(" was loaded succefully\n"); // print succeful loading message

        }
        else if (line[1] == '1') { // S1 record processing
            for (int i = 0; i < dataLength; i += ASCII_SIZE) {
                sscanf(line + HEADER_START + i, "%4x", &data);
                IMEM[(address >> MEM_SHIFT) + (i >> BYTE_SIZE)] = (data >> DATA_SHIFT) | ((data & BYTE_MASK) << DATA_SHIFT); // Correctly handle high and low byte
            }
        }
        else if (line[1] == '2') { // S2 record processing
            for (int i = 0; i < dataLength; i += ASCII_SIZE) {
                sscanf(line + HEADER_START + i, "%4x", &data);
                DMEM[(address >> MEM_SHIFT) + (i >> BYTE_SIZE)] = (data >> DATA_SHIFT) | ((data & BYTE_MASK) << DATA_SHIFT); // Correctly handle high and low byte
            }
        }
        else if (line[1] == '9') { // S9 record processing
            sscanf(line + 4, "%4x", &address); // Read starting address
            printf("Starting address: %04X\n", address); // print starting address
            RegistersValue[PC] = address; // set PC to starting address
        }
    }

    fclose(file);
}

void PrintMEM(unsigned char* MEM) { // was void PrintMEM(unsigned char* MEM, int Range_Start, int Range_End) {
/* 
function is called to print either IMEM or DMEM
- user is prompted to select range of memory to print
- IMEM and DMEM are printed in 16 byte pairs per row or 32 bits per row
- At the end the byte ascii representation is printed
*/
    unsigned short Range_Start, Range_End;

    printf("Enter Memory Range in HEX:");
    scanf("%x %x", (unsigned int *) &Range_Start, (unsigned int *) &Range_End); 

    for (int i = Range_Start; i < Range_End; i += BYTES_PER_LINE) { // Loop from the start-end of mem[range]

        printf("%04X: ", i); // print address

        for (int j = 0; j < BYTES_PER_LINE; j++) { // Loop through each byte in the current memory

            if (i + j < Range_End) { // print if byte is within range
                printf("%02X ", MEM[i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" ");

        for (int j = 0; j < BYTES_PER_LINE; j++) { // Loop through each byte in the current memory word
            if (i + j < Range_End) { // If the current byte is within the memory range
                unsigned char c = MEM[i + j];
                if (c >= ASCII_START_VALUE && c <= ASCII_END_VALUE) { // if within ascii range - print
                    printf("%c", c);
                } else {
                    printf(".");
                }
            } else { // (current byte is outside the memory range)
                printf(" ");
            }
        }
        printf("\n"); // separates words in memory
    }
    printf("\n");

    getchar();
}

unsigned int calculateChecksum(const char* line, int count, int dataLength) {
    unsigned int checksum = 0;
    int i;

    for (i = 2; i < HEADER_START; i += BYTE_SIZE) { // add size and address bytes
        unsigned int byte; 
        sscanf(line + i, "%2x", &byte);
        checksum += byte;
    }

    for (i = HEADER_START; i < HEADER_START + dataLength; i += BYTE_SIZE) { // add data bytes
        unsigned int byte;
        sscanf(line + i, "%2x", &byte);
        checksum += byte;
    }

    checksum = (~checksum) & 0xFF; // check if the checksum is valid
    return checksum;
}
