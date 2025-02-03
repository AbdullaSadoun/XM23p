
;
; Sum an array of 16-bit numbers
; ECED 3403
; 15 May 24
;
	CODE
	org	#1000
;
;
Main	movlz	Array,R0	; r0=Address of the array
	ld	R0,R3		; load stopper into r3
	add	#2,R0		; move r0 to the next element (first element to be summed) increment by 2 as bytes are in pairs
	mov	R1,R2		; setting r2 as sum register and making it 0
;
loop	ld	R0,R1		; load the array's element into r1
	add	R1,R2		; Add the element to the sum
	add	#2,R0		; Increment R0 to point to the next element in the array
;
; check if stopper is 0 to stop summing
;
	sub	#1,R3		; stopper - 1
	bz	Done		; end loop if stopper is 0
;
	bra	loop		; continue adding
;
; adding complete, result are in r2
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
; the array of integers used:
;
Array	word	$-1		; (5=stopper in r3)
	word	#1000
	word	#2000
	word	#3000
    word	#4000
    word	#5000
;
; no store for result they remained in register
;
	end	Main