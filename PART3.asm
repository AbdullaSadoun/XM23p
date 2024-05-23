;
; set bits in a register counter
; ECED 3403
; 15 May 24
;
NUL equ #0
;
	CODE
	org	#1000
;
; Initialize the register whose bits we want to count
; Assume the register R1 contains the value for which we need to count set bits
;
Main    movlz   $0,R0
	ld 	R0,R1	    ; testing register r1
	movlz	$0,R2		; set r2 as 0 (counter) 
    movlz $1,R4
    movlz $0,R0
;
CountBits
	movlz	#16,R3		; r3 = 16 which are the amount of bits in r1
;
CountLoop
	cmp	R0,R1		; Check if R1 is zero
	bz	Done		; if r1 is zero no bits are set
    add	R4,R2		; Increment the count of set bits
    sra R1          ; shift r1 by 1
    sub R4,R3       ; another check to stop looping at 16bits
    cmp R0,R3       ; ""
    bz Done
    bra CountLoop   ; if program got here then it should repeat loop
Done
;
BWait
	bra	BWait			; Infinite loop
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data space 
;
	DATA
	org	#40
;
	end	Main
