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
; testing PSW
	setcc	vsnzc ; check to see if the instruction works (PSW:11111)
	clrcc	vsnzc ; check to see if the instruction works (PSW:00000)
;
; setting PSW for cex testing
    setcc   nzc ; Set N, Z, C flags (PSW: 00111)
                ; EQ, CS, MI, LE, VC should be true
                ; NE, PL, CC, GE, LT, GT should be false
;
    cex     eq,$2,$2 ; eq is true so r0=ffff
    movl    V1,R0; instruction to get r0.l to be 0xff if true
    movh    V1,R0; instruction to get r0.h to be 0xff if true
    movl    V1,R1; instruction to get r1.l to be 0xff if true
    movh    V1,R1; instruction to get r1.h to be 0xff if true 
;   
end MAIN