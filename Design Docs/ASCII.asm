;
; Array accessing using indexed addressing
;
	DATA
	ORG	#40
InArray	ASCII	"ABCD\0"
;
; Put on word (even byte) boundary
;
	align
;
OutArray
	BSS	$10		; 10 bytes or 5 words
;
	CODE
ASCEX
	MOVLZ	InArray,R0	; R0 <- address of InArray
	MOVLZ	OutArray,R1	; R1 <- address of OutArray
;
	LD.B	R0+,R2		; 'A'
	ST	R2,R1+
	LD.B	R0+,R2		; 'B'
	ST	R2,R1+
	LD.B	R0+,R2		; 'C'
	ST	R2,R1+
;
; Breakpoint here
;
	MOV	R2,R2
	end	ASCEX
