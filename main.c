#include "loader.h"

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[1], "r"); // openning input file
    if (fin == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    // reading the file line by line and print lines read
    char line[MAX_S_RECORD_SIZE];
    while(fgets(line, MAX_S_RECORD_SIZE, fin) != NULL){
        printf("%s", line);
        // process each s-record (line)

    }

    printf("Program Ran succefully\n");

    return 0;
}