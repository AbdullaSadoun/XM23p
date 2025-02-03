/*
Loader for XM-23p
- takes in .xme files and loads them into memory

Written by: Abdulla Sadoun
*/

#include "xm23p.h"


int main(int argc, char* argv[]){

    char choice; // char to temporarily store choice of the user

    memset(IMEM, '0', sizeof(IMEM)); // initializing IMEM to 0s
    memset(DMEM, '0', sizeof(DMEM)); // initializing DMEM to 0s

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 16; j++){
            RegistersBinaryString[i][j] = '0';
        }
        for(int j = 0; j < 4; j++){
            RegistersHexString[i][j] = '0';
        }
    }

    //int timecounter = 0; // time counter

    while(choice != 'q'){ // while user does not choose to quit
        printf("===========MENU===========\n");
        printf("l - Load file\n");
        printf("m - Print memory\n");
        printf("f - Fetch (BETA)\n");
        printf("d - Debug (BETA)\n");
        printf("q - Quit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        if(choice == 'l'){ // user chooses to load file
            char filename[150];
            printf("Enter filename: ");
            scanf("%s", filename);

            FILE *fin = fopen(filename, "r"); // opening input file
                if (fin == NULL) {
                    printf("File Open Error\n");
                    break;
                }

            // reading the file line by line and print lines read
            char line[MAX_S_RECORD_SIZE];
            while(fgets(line, MAX_S_RECORD_SIZE, fin) != NULL){
                ProcessSRecords(line); // process each s-record (line)
            }

            fclose(fin); // close the input file

        } else if(choice == 'm'){ // user chooses to print memory
            char memchoice;
            printf("select Memory I=IMEM D=DMEM B=both\n");
            scanf(" %c", &memchoice);

            if(memchoice == 'I' || memchoice == 'i'){ // print IMEM
                PrintIMEM();
            } else if(memchoice == 'D' || memchoice == 'd'){ // print DMEM
                PrintDMEM();
            } else if(memchoice == 'B' || memchoice == 'b'){ // print both
                PrintMEM();
            } else { // invalid choice
                printf("Invalid choice\n");
            }

        } else if(choice == 'q'){ // user chooses to quit
            return 0;
        } else if(choice == 'f'){ // user chooses to fetch
            process_instruction(); 
            
        } else if(choice == 'd'){ // user chooses to fetch
            debug(); // debug function
        } else { // invalid choice
            printf("Invalid choice\n");
        }
    }
    
    return 0;
}