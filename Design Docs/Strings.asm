;
; Example X-Makina program
; Assembling two string functions (strlen() and strcpy())
; Called by mainline
;
; L. Hughes
;
; 11 May 2023 - XM23 upgrade
; 25 Mar 2020 - XM3 upgrade
; 28 May 2019 - Rewrite to XM2
; 11 Jun 2018 - Second example of storing length using relative addressing
; 11 May 2018 - Initial release
;
STKTOP	equ	#FFC0	; Stack top - PUSH (decr then write); PULL (read then incr)
NUL	equ	'\0'	; End-of-string indicator
; 
r2	equ	R2	; r2 is alias of R2
LR	equ	R5	; LR is alias of R5
SP	equ	R6	; SP is alias of R6 
PC	equ	R7	; PC is alias of R7
;
; Code space
;
	org	#1000
Main
;
; Initialize stack point
;
	movls	STKTOP,SP	; STKTOP.LSB to SP.LSB; $FF to SP.MSB
;
; R1 points to string
;
	movl	MyStr,R1
	movh	MyStr,R1
;
; Call strlen()
; R1 points to string
; R0 returned with length
;
	bl	strlen	
;
; Two examples of storing (R0, length, in MSLen):
;
; 1. Indexed using R1 (address of MyStr)
;
	sub	#2,R1	; Word before MyStr is MSLen
	st.w	R0,R1+	; Save R0 in MSLen
			; Bit of a cheat - increment R1 by 2 to point 
			; back to string MyStr
;
; 2. Relative using offset "back" from R1 to MSLen's location
;
	str.w	R0,R1,$-2	; mem[R1-2] = R0
				; Note that R1 is unchanged
;
; Call strcpy()
; MyStr (R1) to Str2 (r2)
; Setup destination str in R2
;
	movl	Str2,r2
	movh	Str2,r2
;
	bl	strcpy	; LR holds return address
;
; No halt, loop until ^C
;
Done	
	bra	Done
;
; String length function
; Input: R1 - Address of NUL-terminated string
; Returns: Length of string in R0
;
strlen
; 
	st	R1,-SP	; Push R1 (to restore on exit)
	st	R2,-SP	; Push R2 (holds character read)
	movlz	#0,R0	; Return length in R0 (initially zero)
;
strlen1	
	ld.b	R1+,R2	; Copy byte [R1] to R2 and R1 incremented by 1
	cmp.b	NUL,R2	; Compare byte (in R2) with NUL
	beq	strlen2	; If equal, go to strlen2
	add	#1,R0	; R0 (length) incremented by 1
	bra	strlen1	; Check next byte
;
; Finished
;
strlen2	
	ld	SP+,R2	; Pull R2
	ld	SP+,R1	; Pull R1
	mov	LR,PC	; Return to caller.  R0 has length
;
	org	#2000
;
; String copy function
; Input: R1 - src string; R2 - dst string
; R0 holds char - stop when NUL
; No check on length of dst string
; Returns: nothing
;
strcpy
	st	R0,-SP	; Push R0
	st	R1,-SP	; Push R1
	st	R2,-SP	; Push R2
;
	movlz	#0,R3
;
strcpy1
	add	#1,R3
	ld.b	R1+,R0	; ch = src[i++]
	st.b	R0,R2+	; dst[j++] = ch (includes NUL)
	and.b	$-1,R0
;
; ch = NUL? - AND sets PSW flags
; No - do next ch
;
	bne	strcpy1
;
; NUL found - restore registers and return
;
	ld	SP+,R2	; Pull R2
	ld	SP+,R1	; Pull R1
	ld	SP+,R0	; Pull R0
	mov	LR,PC	; return to caller
;
; Data space 
; Setting the origin could be unsafe - if code addresses extend past $1000, they
; could be overwritten by the data.  This is one argument for a linker.
;
	org	#3000
;
MSLen	bss	#2	; Reserve 2 bytes (word) for length of MyStr
;
MyStr	ascii	"The cat sat on the mat\n\0"
;
Str2	bss	$100	; 100 bytes for destination string
;
	end	Main