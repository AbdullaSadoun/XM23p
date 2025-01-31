		org #1000
MAIN
	setcc	vsnzc
	clrcc	vsnzc 
    setcc   vc
	clrcc   vc
DONE
	bra		DONE			; loop
end MAIN