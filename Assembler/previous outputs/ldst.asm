            ORG 0x0100
DATA_START  DB 'eced3403', 0

; Initialize data memory where data will be copied using LD and ST instructions
            ORG 0x0110
DATA_COPY   DB 8 DUP(?)

; Program starts here
            ORG 0x0200

; Load data from 0x0100 into registers using LD
            MOV R0, 0x0100  ; Load the starting address of the source
            MOV R1, 0x0110  ; Load the starting address of the destination

            LD R2, [R0]+    ; Load byte from source to R2 and increment source address
            ST [R1]+, R2    ; Store byte from R2 to destination and increment destination address

            LD R2, [R0]+    ; Repeat for the next byte
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            LD R2, [R0]+
            ST [R1]+, R2

            HLT             ; Halt the program