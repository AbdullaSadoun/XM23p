/*
Loader for XM-23p
Written by: Abdulla Sadoun
*/

#include "loader.h"


int main(int argc, char* argv[]){

    if (argc < 2){
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r"); // openning input file
    if (fin == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    memset(IMEM, '0', sizeof(IMEM)); // initializing IMEM to 0s
    memset(DMEM, '0', sizeof(DMEM)); // initializing DMEM to 0s

    // reading the file line by line and print lines read
    char line[MAX_S_RECORD_SIZE];
    while(fgets(line, MAX_S_RECORD_SIZE, fin) != NULL){
        ProcessSRecords(line); // process each s-record (line)
        printf("%s", line); // for debugging purposes
    }
    fclose(fin); // close the input file

    PrintMEM(); // print the memory

    printf("\nProgram Ran succefully\n"); // for debugging purposes

    return 0;
}