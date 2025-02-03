;
; Set bits in a register counter using shifts
; ECED 3403
; 15 May 24
;
Count   equ     R0      ; Count used in place of R0
DataVal equ     R1      ; DataVal - R1
One     equ     R2      ; One constant - R2
;
	CODE
	org     #1000
Start
;
; Initialize the register whose bits we want to count
; Assume the register R1 contains the value for which we need to count set bits
;
	movlz   $0xFFFF,DataVal  ; Load test value directly into DataVal (R1)
	movlz   $0,Count         ; Count = 0
	movlz   $1,One           ; One constant for incrementing and bit masking
;
; Check bits until all 16 bits are processed
;
	movlz   #16,R3           ; Initialize R3 with 16, the number of bits in R1
BC_Loop
	mov     DataVal,R4       ; Copy DataVal to R4 for manipulation
	and     One,R4,R4       ; Isolate the lowest bit of DataVal
	cmp     #1,R4            ; Compare the isolated bit with 1
	beq     IncrementCount    ; If the lowest bit is 1, increment the count
;
ShiftRight
	sra     DataVal           ; Arithmetic shift right by 1, move to the next bit
	sub     #1,R3            ; Decrement the bit count
	cmp     #0,R3            ; Check if all bits have been processed
	bne     BC_Loop           ; Continue looping if not all bits processed
	bra     Done              ; If all bits have been processed, go to Done
;
IncrementCount
	add     One,Count        ; Increment the count of set bits
	bra     ShiftRight        ; Continue checking the next bit
;
Done
	bra     Done              ; Infinite loop (or replace with a suitable end)
;
; Data space (if needed, remove or comment out if using direct loading)
;
	DATA
	org     #2000
Data    word    #FFFF         ; Example data value to be counted
;
	end     Start
