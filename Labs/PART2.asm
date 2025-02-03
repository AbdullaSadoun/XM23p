;
; LowerToUpper ASCII converter
; ECED 3403
; 15 May 24
;
NUL equ #0 
;
	CODE
	org	#1000
;
Main	movlz	InStr,R0	; r0<-input string address
;
loop	ld.b	R0,R1		; r1<-array element
	cmp.b	NUL,R1		; NUL char. check
	bz	Done	; skip loop if NUL found
;
; load constants into registers for comparison
movlz	'a',R2	; used to check within range
movlz	'z',R3	; ""
movlz	$32,R4	; 32 which offsets lower/upper in ascii
;
; check if lower (must be between a and z)
cmp.b	R2,R1	; compare element with a
blt	NextChar	; skip if less that a
cmp.b	R3,R1	; compare element with z
bge	NextChar	; skip if greater than z
;
; convert current element to upper
	sub	R4,R1	; subtract offset to get upper
	st.b	R1,R0		; save new char.
;
NextChar
	add	#1,R0		; increment bit for next element check
	bra	loop		; loop through directives
;
Done
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
; input string
;
InStr	ascii	"these are my characters !@#$"	
byte	NUL
;
;	org	#80
;OutStr	ascii	"#############################################"
;
;
	end	Main
