
	data
	org	#10
V1	word	#FFFF
;
	code
	org	#200
LDST1
;
; Get address of V1 into R0
;
;	movl	V1,R0		; R0 = #??00
;	movh	V1,R0		; R0 = #0010
;
; R1 <- mem[R0]
;
; R0 contains the effective address
; mem[R0] is rvalue (a value or expression)
; R1 is lvalue (where the result is to be stored)
;
; LD from-memory (rvalue),to-register (lvalue)
;
;	LD	R0,R1		; R1 <- #FFFF
	LDR R0,#16,R1
;
;	ADD	R0,R0		; R0 <- R0 + R0
;
; mem[R0] <- R1
;
; R1 is rvalue
; R0 is the effective address
; mem[R0] is lvalue
; 
	STR	R1,R0,#1
;
; Put breakpoint on next address
;
BrkPtHere
;
	end LDST1
