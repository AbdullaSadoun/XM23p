;
; Test of A3 and A4
; A3: Memory reads and writes
; A4: Branching
;
PC	equ	R7
SP	equ	R6
LR	equ	R5
NUL	equ	'\0'
;
; Data segment
;
	Data
	org	#60
InStr	ascii	"Input string to copy\0"
	org	#80
OutStr	ascii	"xxxxxxxxxxxxxxxxxxxxxxxx"
;
; Stack segment
;
	org	#FE00
	bss	#100
StkTop	
;
; Code segment
;
	Code
	org	#100
;
; ReadSubr - returns a char from R1 in R0
;
ReadSubr
	ld.b	R1+,R0		; Read R1 into R0
	movh	#FF00,R0	; Overwrite MSByte
	mov	LR,PC		; Return
	add	#FF,PC		; Must not execute
;
; Mainline
;
RWStr
	movl	StkTop,SP
	movh	StkTop,SP
;
	movlz	InStr,R1
	movlz	OutStr,R2
;
RWLoop
	bl	ReadSubr
	st	R0,-SP		; Char as parameter
	st	R2,-SP		; Addr as parameter
;
	bl	WriteSubr
	ld	SP+,R2		; Restore Addr
	ld	SP+,R0		; Restore Char
;
	cmp.b	NUL,R0
	bne	RWLoop		; Not NUL, repeat
;
; NUL found, infinite loop until ^C
;
RWDone	bra	RWDone
;
; Writes a character to the destination string
; Stack has Output address and char to output
;
WriteSubr
	ldr	SP,$0,R0	; R0 <- Addr of output string
	ldr	SP,$+2,R2	; R2 <- Character to output
;
	st.b	R2,R0
;
	add	#1,R0		; Increment address
	str	R0,SP,$0
;
	mov	LR,PC		; Return
	add	#FF,PC		; Must not execute
;
	end	RWStr
