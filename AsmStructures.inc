
CpuCell struc
		Cpueax dword 0
		Cpuebx dword 0
		Cpuecx dword 0
		Cpuedx dword 0
CpuCell ends

CpuData struc
cpu CpuCell 32 dup ({0,0,0,0})
CpuData ends

MULTIPLY_FPDOUBLE struc
		AmountOfDoubles dq 0
		ArithmeticType	dq 0
		ActionOnType    dq 0
		SpecicalAction	dq 0
		InputBufferOne	real8 0.0,0.0,0.0,0.0
		InputBufferTwo	real8 0.0,0.0,0.0,0.0
		OutPutBuffer	real8 0.0,0.0,0.0,0.0
	MULTIPLY_FPDOUBLE ends

	 CPUFLAGS struc
	 fFpu		db 0		;1
	 fMmx		db 0		;2
	 fSse		db 0		;3
	 fSse2		db 0		;4
	 fSse3		db 0		;5
	 fSsse3		db 0		;6
	 fSse4_1	db 0		;7
	 fSse4_2	db 0		;8
	 fAvx		db 0		;9
	 fFma		db 0		;10
	 fAesni		db 0		;11	
     fProsn     db 0		;12
     fF16c		db 0		;13
	 fRand      db 0		;14
	 spare4     db 0		;15
	 spare5     db 0		;16
	CPUFLAGS ends

	SYSTEM_INFO_DETAILS struc
	dwOemId			dd 0
	dwPageSize		dd 0
	lpMinAppAdress	dq 0
	lpMaxAppAdress	dq 0
	lpActiveProMask	dq 0
	dwAmountOfPros	dd 0
	dwProType		dd 0
	dwGran			dd 0
	dwProcLv1		dw 0
	dwProcRev		dw 0
	SYSTEM_INFO_DETAILS ends