;
; Test of A5 (Conditional Execution)
; BRA test 
; Tests disabling cex after BRA execution
;
; data segment
; FFFF stored in DMEM[FFFF]
    data
    org #FFFF
V1	word	#FFFF
;
;
;code segment
    code
    org #1000
MAIN
;
; ensure psw is cleared
	clrcc	vsnzc ; check to see if the instruction works (PSW:00000)
;
;
   setcc   nzc ; Set N, Z, C flags (PSW: 00111)
                ; EQ, CS, MI, LE, VC should be true
                ; NE, PL, CC, GE, LT, GT should be false
;
; testing EQ - true
    cex     eq,$2,$2 ; eq is true
    bra     SETR0; subr to set (r0=FFFF if true) (should execute)
    movl    V1,R1; instruction to get r0=00FF if true and executed (shouldnt execute)
    movl    V1,R2; instruction to get r2=00FF if false (shouldnt execute)
    movh    V1,R2; instruction to get r2=FF00 if false (shouldnt execute)
; expected everything after bra shouldnt execute, output will be in r0 
; r0: FFFF
; r1: 0000
; r2: 0000
;
    FAILED  bra FAILED ;infinite loop if test fails
;
; the following subr should execute proving our fault handler works
SETR0
    movl    V1,R0; set r0 to 00FF
    movl    V1,R0; set r0 to FFFF
;
;
; Terminate the program
    end MAIN