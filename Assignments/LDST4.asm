;
; Example of index addressing #2
; Load and store using bytes and indexing
; This is an example of a post-increment
;
; Copy three bytes from S1 to S2
;
	data
	org	#100
S1	ascii	"abc"
	org	#110
S2	ascii	"XXX"
;
	code
	org	#200
LDST4
;
; Get address of V1 into R0
;
	movl	S1,R0		; R0 = #??00
	movh	S1,R0		; R0 = #0100
;
; Get address of V1 into R1
;
	movl	S2,R1		; R1 = #??10
	movh	S2,R1		; R1 = #0110
;
; Repeat (without loop):
;
; mem[R1] <- mem[R0]
;
; R0, R1 contain effective addresses
; Need a temporary (use R2)
; R2 <- mem[R0]
; mem[R1] <- R2
; 
; Do this 3x:
; 110 <- 100
; 111 <- 101
; 112 <- 102
;
	LD.B	R0+,R2		; R1 <- mem[R0]
	ST.B	R2,R1+		; mem[R1] <- R1
;
	LD.b	R0+,R2		; R1 <- mem[R0]
	ST.b	R2,R1+		; mem[R1] <- R1
;
	LD.B	R0+,R2		; R1 <- mem[R0]
	ST.b	R2,R1+		; mem[R1] <- R1
;
; Put breakpoint on next address
;
BrkPtHere
;
	end LDST4
