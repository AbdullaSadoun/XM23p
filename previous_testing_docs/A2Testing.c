/*
Test 1: Testing Arithmetic functions (ADD)
Purpose/Objective: To test the arithmetic function ADD, to ensure that it is working correctly. this test will also test the fetch, decode, execute and PSW functions.
Test Configuration: I will use the ADD function to add two numbers together using the following code:
I will use the following code from ADD-ADDC.asm		

org #1000
MAIN
	movh	#9000,R0
	movh	#9000,R1
	add	    R1,R0	    ;carry and overflow flag using word addition
	addc	$0,R0       ;adding 0 to a register with carry of 1, clearing flags
	movl	$1,R0
	movl	$-1,R1
	add.b	R1,R0	    ;producing a zero flag using byte addition
	add.b	R1,R0	    ;producing a negative flag
DONE
	bra		DONE		; loop
end MAIN

Expected Results: I expect the code to run properly and produce the correct flags for the ADD function. I expect the carry flag to be set after the first addition 
-----------------Actual Results: 
-----------------Pass/Fail: Pass
*/
/*
Test2 : Testing Arithmetic Functions with carry (ADDC)
Purpose/Objective: To test the arithmetic function ADDC, to ensure that it is working correctly. this test will also test the fetch, decode, execute and PSW functions.
Test Configuration: I will use the ADDC function to add two numbers together using the same code and I will use debug mode to step through and check the change in registers and PSW flags
I will use the following code from ADD-ADDC.asm
org #1000
MAIN
	movh	#9000,R0
	movh	#9000,R1
	add	    R1,R0	    ;carry and overflow flag using word addition
	addc	$0,R0       ;adding 0 to a register with carry of 1, clearing flags
	movl	$1,R0
	movl	$-1,R1
	add.b	R1,R0	    ;producing a zero flag using byte addition
	add.b	R1,R0	    ;producing a negative flag
DONE
	bra		DONE		; loop
end MAIN
Expected Results: I expect the code to run properly and produce the correct flags for the ADDC function. I expect the carry flag to be set after the first addition and cleared after the second addition.
-----------------Actual Results: 
-----------------Pass/Fail: Pass
*/
/*
Test3 : Testing Arithmetic function (AND)
Purpose/Objective: To test the arithmetic function AND, to ensure that it is working correctly. this test will also test the fetch, decode, execute and PSW functions.
Test Configuration: I will use the AND function to AND(&) two numbers together using the following code and I will use debug mode to step through and check the change in registers and PSW flags
AND.asm
		org #1000
MAIN
	MOVH	#FFFF,R0
	MOVL	#FF,R0
	MOVL	#08,R1
	AND 	R0,R1		;produce no flags using byte and register source
	MOVL 	#F0,R2
	AND.B	R0,R2		;produce negative flag using word and register source	
	AND 	$0,R0		;produce zero flag using word and constant source
DONE
	bra	DONE		; loop
end MAIN
Expected Results: I expect the code to run properly and produce the correct flags for the AND function. I expect the zero flag to be set after the third AND operation and the negative flag to be set after the second AND operation.
-----------------Actual Results:
-----------------Pass/Fail: Pass
*/
/*
Test4 : Testing MOVL & MOVH
Purpose/Objective: To test the MOVL and MOVH functions, to ensure that they are working correctly. this test will also test the fetch, decode, execute and PSW functions.
Test Configuration: I will use the MOVL and MOVH functions to move a value into a register using the following code and I will use debug mode to step through and check the change in registers and PSW flags
MOVL-MOVH.asm
		org #1000
MAIN
	movh	#EE00,R0
	movl	#99,R0		;set low bytes
	movh	#AA00,R1	;set high bytes
	movlz	#88,R2		;set low bytes without changing high bytes
	movls 	#77,R3		;set low bytes and clear high bytes
DONE
	bra		DONE			; loop
end MAIN
Expected Results: I expect the code to run properly and produce the correct flags for the MOVL and MOVH functions. I expect the low bytes to be set after the first MOVL and the high bytes to be set after the first MOVH. I expect the low bytes to be set after the first MOVL and the high bytes to be cleared after the first MOVLS.
-----------------Actual Results:
-----------------Pass/Fail: Pass
*/
/*
Test5 : Testing Register Functions (BIT)
Purpose/Objective: The purpose of this test is to test the BIT instruction, I have chosen this instruction because it is an important instruction that can be used to test the register functions of the emulator without going into debug mode by the programmer.
Test Configuration: I will use the BIT function to test the bits of a register using the following code and I will use debug mode to step through and check the change in registers and PSW flags
BIT.asm

-----------------Actual Results:
-----------------Pass/Fail: 
*/
/*
Test6 : Testing Register Functions (CMP)
Purpose/Objective: The purpose of this test is to test the CMP instruction, I have chosen this instruction because it is very common and used alot in programming for comparison which is a fundamental part of programming.
Test Configuration: I will use the CMP function to compare two numbers using the following code and I will use debug mode to step through and check the change in registers and PSW flags.
CMP.asm
		org #1000
MAIN
	MOVL	#08,R2
	MOVL	#08,R1
	CMP.b 	R1,R2		;0x08-0x08
	MOVH	#0800,R1
	MOVL	#00,R1
	MOVH	#0700,R2	;0x0700-0x0800
	CMP 	R1,R2
	MOVL	#09,R3
	CMP 	$1,R3		;0x0009-$1
DONE
	bra	DONE		; loop
end MAIN

Expected Results: I expect the code to run properly and produce the correct flags for the CMP function. I expect the zero flag to be set after the first CMP operation and the negative flag to be set after the second CMP operation.
-----------------Actual Results:
-----------------Pass/Fail: 
*/