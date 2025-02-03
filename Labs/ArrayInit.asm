;
; Sample X-Makina program
; Initialize a block of memory to 'A' through 'Z'
;
; L. Hughes
;  3 May 24: Update to XM23p
; 15 May 18: Comment correct R1 = 'Z'
;  1 May 18: Initial release
;
SIZE	equ	$26
CAP_A	equ	'A'
CAP_Z	equ	'Z'
;
; Start of data area
;
	Data
	org	#800
Base	bss	SIZE		; Reserve SIZE bytes
; Start of code area
	Code
	org	#1000
;
Main
	movlz	'A',R0		; R0 = 'A'
	movlz	CAP_Z,R1	; R1 = 'Z'
	movl	Base,R2		; R2 = Base (Address for chars)
	movh	Base,R2
; 
Loop
	st.b	R0,R2+		; [R2] = R0; R2 = R2 + 1
	cmp.b	R0,R1		; R0 = R1 ('Z')
	beq	Done		; If equal, goto Done
	add.b	$1,R0		; No: R0 = R0 + 1 (next ASCII char)
	swap	R0,R0		; NOP to test count
	bra	Loop		; Repeat loop
; End of program
Done	movlz	'*',R1
	cmp.b	R1,R1
;
	end	Main		; End of program - Code begins at "Main"
