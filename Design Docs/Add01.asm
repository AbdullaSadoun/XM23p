;
; Simple addition
;
	org	#100
Start
;
; Initialize registers
;
	movlz	#20,R1	; R1 <- #0020
	movlz	#F5,R2	; R2 <- #00F5
;
	add	R1,R2	; R2 <- R2 + R1
;
	end	Start
