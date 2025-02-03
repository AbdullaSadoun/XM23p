;
; Addition examples
;
; ECED 3403
;  9 May 24 - Original
;
	org	#1000
;
; Add a value to a register
;
Main
	movlz	$15,R0	; R0 <- 15
	add	#1,R0	; R0 <- R0 + constant 1
;
; Add two registers
;
	movlz	#F0,R1	; R0 <- #F0
	add	R0,R1	; R1 <- R1 + R0 (16 or #10)
;
; Load and store results from memory
;
; R1 <- address of InVal
;
	movl	InVal,R1
	movh	InVal,R1
;
; Read InVal into R0
;
	ld	R1,R0	; R0 <- mem[R1]
;
	add	#2,R0	; R0 <- R0 + 2
;
; Write R0 to OutVal (InVal + 2)
;
	add	#2,R1
	st	R0,R1	; mem[R1] <- R0
;
; All done - busy wait for ^C
;
Done	bra	Done
;
; Data
;
	org	#10a0
;
InVal	word	#0FFE
;
OutVal	bss	$2	; Reserve two bytes
;
	end	Main	; Start of program (S9 record)
