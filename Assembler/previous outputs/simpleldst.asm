; Initialize data memory with a single word value
; Data memory starts at address 0x0100
            ORG 0x0100
DATA_START  WORD    #1234  ; Initialize memory at 0x0100 with the value 0x1234

; Program starts here
            ORG 0x0200

SimpleLDST:
; Get address of DATA_START into R0
            MOVL    #0100, R0  ; Load the lower part of the address 0x0100 into R0
            MOVH    #0000, R0  ; Load the higher part of the address 0x0000 into R0 (R0 = 0x0100)

; Load value from memory into R1
            LD      R0, R1     ; Load the value from address in R0 (0x0100) into R1

; Increment the value in R1
            ADD     #1, R1     ; Add 1 to the value in R1

; Store the updated value back into memory
            ST      R1, R0     ; Store the value from R1 back to the address in R0 (0x0100)

; Halt the program
            HLT                 ; Halt the program

            END SimpleLDST
