#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1<<16
#define MAX_S_RECORD_SIZE 68

//initialize IMEM and DMEM (64kib each)
unsigned char IMEM[MEMORY_SIZE];
unsigned char DMEM[MEMORY_SIZE];

/*functions*/
void ProcessSRecords(char line[MAX_S_RECORD_SIZE]);
void Send2IMEM();
void Send2DMEM();
void PrintMEM();

