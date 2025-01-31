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
    cex     eq,$2,$2 ; eq is false so r1=ffff
    movl    V1,R0; instruction to get r0.l to be 0xff if true
    movh    V1,R0; instruction to get r0.h to be 0xff if true
    movl    V1,R1; instruction to get r1.l to be 0xff if false
    movh    V1,R1; instruction to get r1.h to be 0xff if false
;   
end MAIN