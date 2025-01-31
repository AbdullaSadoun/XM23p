;
; Test of A5 (Conditional Execution)
; Tests multiple conditions
; Tests true and false
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
    cex     eq,$1,$1 ; eq is true so r0=00ff
    movl    V1,R0; instruction to get r0=00FF if true
    movh    V1,R0; instruction to get r0=FF00 if false
; expected R0:00FF
;
;
; testing NE - false
    cex     ne,$1,$2
    movl    V1,R1; instruction to get r1=00FF if true
    movl    V1,R1; instruction to get r1=FFFF if false
    movh    V1,R1; instruction to get r1=FFFF if false
; expected R1:FFFF
;
;
; testing LS - true
    cex     ls,$2,$1
    movl    V1,R2; instruction to get r2=FFFF if true
    movh    V1,R2; instruction to get r2=FFFF if true
    movl    V1,R2; instruction to get r2=00FF if false
; expected R2:FFFF
;
;
; testing LS - true
    cex     ls,$2,$2
    movl    V1,R3; instruction to get r3=00FF if true
    add     R3,R3; instruction to get r3=01FE if true
    movl    V1,R3; instruction to get r3=00FF if false
    movh    V1,R3; instruction to get r3=FFFF if false
; expected R3:01FE
;   
;
; testing TR - always true
    cex     ls,$3,$2
    movl    V1,R4; instruction to get r4=00FF if true
    add     R4,R4; instruction to get r4=01FE if true
    add     R4,R4; instruction to get r4=03FE if true
    movl    V1,R4; instruction to get r4=00FF if false
    movh    V1,R4; instruction to get r4=FFFF if false
; expected R4:03FE
;
;
;
end MAIN