;
; 16bit array summer
; ECED 3403
; 15 May 24
;
NUL	equ	#0
;
	CODE
	org	#1000
;
Main	movlz	Array,R0	; R0 <- load arrray into r0
	movlz	OutStr,R1	; R1 <- 0080
;
Copy	ld.b	R0,R2		; ch <- mem[R0]
	cmp.b	NUL,R2		; ch = NUL?
	beq	CDone		; Yes: ==> Done
;
	st.b	R2,R1		; mem[R1] <- ch
;
; Next locations
; There is a "better" way of doing this
;
	add	#1,R0
	add	#1,R1
;
	bra	Copy
;
; NUL found - write to outstr.
; R1 points to last location
;
CDone
	st.b	R2,R1		; R2 is NUL
;
; Finished - busy wait
;
BWait	bra	BWait
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data space
;
	DATA
	org	#40
;
; Input string
;
InStr	ascii	"This is an ASCII string"
	byte $0
;
; Output string
; Use ASCII instead of BSS to check for NUL terminator
;
	org	#80
OutStr	ascii	"#############################################"

;
	end	Main
	
