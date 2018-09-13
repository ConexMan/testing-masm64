extern Showcpu:proc				;tell masm that there is a external proc
extern CreateThread:proc
extern GetSystemInfo:proc
extern malloc:proc
extern free:proc
extern QueryPerformanceFrequency:proc
extern QueryPerformanceCounter:proc
extern printf:proc
include AsmStructures.inc
public bytes16

.data
align(16)
realVal		real8 +1.5				; this stores a real number in 8 bytes
RealAdd		real8 +21.1
real_hd		real8 +1560.34561
real_ld     real8 +6430.7678
real4_a     real4 +12.5
real4_b     real4 +208.6
real4_c     real4 -1204.5
real4_d     real4 +2082.65
bytes16     db 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
avg16		db 10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160
mask1		dq 8000000000000000h,8000000000000000h,0,0
mask3		dd 80000000h,80000000h,80000000h,80000000h,80000000h,80000000h,80000000h,80000000h
index1      qword 0,1,0,0
mask2       db 00h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h,80h
indices     dd 7,6,5,4,3,2,1,0
;*************************** used for temp storing the regs **************

Temprax		qword 0
Temprbx		qword 0
Temprcx		qword 0
Temprdx		qword 0
TempR8		qword 0
TempR9		qword 0
TempR10		qword 0
TempR11		qword 0
TempR12		qword 0
TempR13		qword 0
TempR14		qword 0
TempR15		qword 0
Tempeax		dword 0
Tempebx		dword 0
Tempecx		dword 0
Tempedx		dword 0
TSstart     qword 0
TSend       qword 0
TSdiff      qword 0
;****************************************************************************
mes1		db 'This Hello is From mes1 ASM Brian',0
mes2		db 'This Hello Is From mes2 Brian %8.6f and %8.2f',0dh,0ah,0
MaxString   db 40h
EntryEax	dword 0
pad         db 12h
pad1        qword 01234h

align(16)
pfp1			real8 5.8,10.8,125.8,1109.8
pfp5			real8 51.8,110.8,11425.8,1079.86
Frequency       dq 0
PerFrequency	real8 0.0
PerMinTime		real8 1.0
MinPeriod		real8 0.0
PerTime1ms		real8 0.001
ExtraCounts		dq 0
StampStart		dq 0
StampEnd		dq 0
perStartCount   dq 0
inLoopCount     dq 0
PerEndCount		dq 0
TargetCounts    dq 0
fpone		real8 10.0
myinxmm0	real8 0.0
myinxmm1    real8 0.0
myMemory    qword 0
align 16
oldxmm0		real8 ?
oldxmm1		real8 ?
amountcpu	dword 0
amountex	dword 0
mutex		dw    0
xmmred      xmmword ?
;cpulist     CpuData{}
simddata    db 0
bytepos		dq 0
;************************************************************
;using rcx = StartTimeStamp
.code
public GetStartTimeStamp
GetStartTimeStamp proc
rdtsc
rol rdx,32
add rdx,rax
mov qword ptr[rcx],rdx
ret
GetStartTimeStamp endp
;************************************************************
;using rcx = EndTimeStamp: rdx = AmountStamp: r8 = StartTimeStamp
public GetEndTimeStamp
GetEndTimeStamp proc
push rdx						;need to save rdx as it points to AmountStamp
rdtsc
rol rdx,32
add rdx,rax
mov qword ptr[rcx],rdx			;End of Stamp count
mov r9,rdx						;End of Stamp count stored in r9
pop rdx							;restore rdx points to AmountStamp
mov rax,qword ptr[r8]           ;get into rax the value of StartTimeStamp   
sub r9,rax                      ;subtract from r9(EndTimeStamp) - rax(StartTimeStamp)
mov qword ptr[rdx],r9			;place answer into rdx(AmountStamp)   
ret
GetEndTimeStamp endp
;************************************************************
;using rcx = EndTimeStamp: rdx = AmountStamp: r8 = StartTimeStamp
public GetTicksForOneMs
GetTicksForOneMs proc
mov temprcx,rcx					;save rcx points to dos.StampsFor1Ms (ULARGE_INTEGER)
mov temprbx,rbx
lea rcx,Frequency				;set rcx(par 1) to point to dq Frequency
sub rsp,20h						;allow shadow space 0f 20h
call QueryPerformanceFrequency	;call 
add rsp,20h						;restore the rsp after call
cmp rax,1						;returns 1 if ok
je ok_timer
mov rcx,temprcx					;restore rcx and set dos.StampsFor1Ms (ULARGE_INTEGER) = 0
mov qword ptr[rcx],0
mov al,0						;return false
ret
ok_timer:						;if here got frequency
lea rcx,PerMinTime				;PerMinTime set to real8 1.0
movlpd  xmm0,real8 ptr[rcx]		;xmm0 now set to 1.0
cvtdq2pd xmm1,Frequency			;xmm1 now set to real8 from (integer)Frequency
divsd xmm0,xmm1					;xmm0 now = 1/Frequency this is the time for each count
movapd xmm1,xmm0				;xmm1 now = xmm0
movlpd xmm0,real8 ptr[PerTime1Ms]	;xmm1 now set to 0.001
divsd xmm0,xmm1					;xmm0 now set to amount of counts for 1ms
cvtsd2si rax,xmm0				;convert Scalar (DP) from source xmm0 to dst scalar integer (DQ) rax
mov ExtraCounts,rax				;now place this into ExtraCounts this is the value to add to start
lea rcx,PerStartCount			;PerStartCount is the the value of counter at the start
sub rsp,20h						;allow shadow space
call QueryPerformanceCounter	;call
add rsp,20h						;restore rsp
mov rax,qword ptr PerStartCount	;mov into rax the StartCount
mov rbx,ExtraCounts				;add to this the count needed to wait 1 ms
add rax,rbx
mov TargetCounts,rax			;place this value into TargetCounts
rdtsc							;get tick into edx:eax
rol rdx,32						;move rdx so that rdx = rdx(MSB)rax(LSB) 
add rdx,rax
mov qword ptr StampStart,rdx	;move this into StampStart
loopWait:						;enter loop waiting for 
lea rcx,inloopcount
sub rsp,20h
call QueryPerformanceCounter
add rsp,20h
mov rax,qword ptr inloopcount
cmp rax,qword ptr TargetCounts  ;compares the first source operand with the second operand
jbe loopwait					;this is done by sub the second from the first

rdtsc							;returns EDX:EAX
rol rdx,32						;get ticks into rdx
add rdx,rax
mov qword ptr StampEnd,rdx		;store the ticks at the end of waiting 1ms	
mov rax,StampStart				;move into rax the value at the start of waiting 1ms
sub StampEnd,rax				;subtract from StampEnd - rax(StampStart)
mov rax,StampEnd				;StampEnd now contains the amount ot ticks in 1ms
mov rcx,temprcx					;restore rcx (points first par this was dos.StampsFor1Ms)
mov [rcx],rax					;write this value into the class variable StampsFor1Ms
mov rbx,temprbx					;restore rbx (must be save)
mov al,1						;return true
ret
GetTicksForOneMs endp

;***********************************************************

;FuncAvx(doublesIn, doublesOut, floatsIn, floatsOut);
;8 doubleIn(s):8 doublesOut:16 floatsIn:16 floatsOut
;only called if dos.cpuflags.fAvx is true
;rcx = ptr to doublesIn:rdx = ptr to doublesOut:r8 = ptr to floatsIn:r9 = ptr to floatsOut
;Avx code processors only
public FuncAvx
FuncAvx proc
call storeall
;************** Just move FP(DP) from doublesIn [0-3] to doublesOut [4-7] *******
;************** and FP(DP) from doublesIn [0-4] to doublesOut [0-3] *************
vmovdqu ymm0,ymmword ptr[rcx]
vmovupd ymm1,ymmword ptr[rcx + 32]
vmovupd ymmword ptr[rdx],ymm1
vmovupd ymmword ptr[rdx + 32],ymm0
;*********************************************************************************
;revearses the the order of floatsIn [0-7] to [7-0] with the vgatherdps using
;indices set to 7,6,5,4,3,2,1,0 and mask3 sets high order bits of dword in ymm1
vmovdqu ymm2,ymmword ptr indices	;move unaligned packed integers
vmovdqu ymm1,ymmword ptr mask3
vgatherdps ymm0,ymmword ptr[r8 + ymm2 *4],ymm1
vmovdqu ymmword ptr[r9],ymm0
;revearsed and sent back to floatsOut[0=7]
;************ now do more work on floatsOut[8-15] *******************
vmovups ymm1,ymmword ptr[r8 + 32]	;r8 is pointing to floatsIn[0] so move it to floatsOut[8]
vaddps ymm5,ymm0,ymm1				;add to ymm0 (still got value from above Revearse) to floatsIn[8-15]
vmovdqu ymmword ptr[r9+32],ymm5		;store this addtion to floatsOut[7-15] r9 points to floatsOut[0]
call restoreall
ret
FuncAvx endp
;********************************************************************************
;SSE,SSE2,SSE3
;FuncMultiplyFpDouble(struct _MULTIPLY_FPDOUBLE )
public FuncArithmetic
FuncArithmetic proc
call storeall
mov byte ptr[rcx].MULTIPLY_FPDOUBLE.AmountOfDoubles,4
;*********** InputBufferOne[0]-[3] Doubles ************
movdqa  xmm0,xmmword ptr[rcx].MULTIPLY_FPDOUBLE.InputBufferOne
movdqa  xmm1,xmmword ptr[rcx +16].MULTIPLY_FPDOUBLE.InputBufferOne
;****************************************************
;*********** InputBufferTwo[0]-[3] Doubles ***********
movdqa  xmm2,xmmword ptr[rcx].MULTIPLY_FPDOUBLE.InputBufferTwo
movdqa  xmm3,xmmword ptr[rcx +16].MULTIPLY_FPDOUBLE.InputBufferTwo
;***************************************************
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,0
je NoneAction
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,1
je multiply
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,2
je Divide
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,3
je addvalues
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,4
je subvalues
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,5
je maximum
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,6
je minimum
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,7
je root
cmp qword ptr[rcx].MULTIPLY_FPDOUBLE.ArithmeticType,0FFH
je testing
;**************** Do Testing **********************
;MULTIPLY_FPDOUBLE struc
;		AmountOfDoubles dq 0
;		ArithmeticType	dq 0
;		ActionOnType    dq 0
;		SpecicalAction	dq 0
;		InputBufferOne	real8 0.0,0.0,0.0,0.0
;		InputBufferTwo	real8 0.0,0.0,0.0,0.0
;		OutPutBuffer	real8 0.0,0.0,0.0,0.0
;	MULTIPLY_FPDOUBLE ends

testing:
movddup xmm0,real8 ptr pfp1
movddup xmm1,real8 ptr pfp5

jmp loadbuffers

;**************** Do Multiply **********************
multiply:
mulpd xmm0,xmm2
mulpd xmm1,xmm3
jmp loadbuffers
;**************** Do Multiply **********************
Divide:
divpd xmm0,xmm2
divpd xmm1,xmm3
jmp loadbuffers
;**************** Do addition **********************
addvalues:
addpd xmm0,xmm2
movmskpd rax,xmm0						;extracts 2 bit mask into the reg (zero rest) sign bits of xmm0 DP(FP)'s
addpd xmm1,xmm3
movmskpd rax,xmm1						;extracts 2 bit mask into the reg (zero rest) sign bits of xmm1 DP(FP)'s

jmp loadbuffers
;**************** Do Subtraction *******************
subvalues:
subpd xmm0,xmm2
subpd xmm1,xmm3
jmp loadbuffers
;**************** Do Maximum *******************
maximum:
maxpd xmm0,xmm2
maxpd xmm1,xmm3
jmp loadbuffers
;**************** Do Minimum *******************
minimum:
minpd xmm0,xmm2
minpd xmm1,xmm3
jmp loadbuffers
;**************** Do square root *****************
root:
sqrtpd xmm0,xmm2
sqrtpd xmm1,xmm3
jmp loadbuffers
;**************************************************
loadbuffers:
;************* Now Load OutputBuffer **************
movapd xmmword ptr[rcx].MULTIPLY_FPDOUBLE.OutPutBuffer,xmm0
movapd xmmword ptr[rcx+16].MULTIPLY_FPDOUBLE.OutPutBuffer,xmm1
;*************************************************
NoneAction:
call restoreall
ret
FuncArithmetic endp
;********************************************************************************
;SSE2
;CombineC(1, 2, 3, 4, 5, 6.1)) rcx = a,rdx = b, r8 = c, r9 = d, stack = e, stack = f 
;using the RBP to access the stack 
Public CombineA
CombineA proc
call storeall
push rbp						;save the caller bp on the stack rsp now rsp - 8
mov rbp,rsp						;now set the rbp to our rsp points to rbp on the stack
								;stack = RBP(8):Return Address(10h):Shadow space(30h):e:f
push r15						;this only effects the rsp NOT the rbp so the the above line correct
lea r14,localdata
add	ecx,dword ptr [rbp+30h]		;ECX par 1 (a) a int so add from the stack no 5 par (e) a int
add ecx,r9d						;add to ECX the dword at R9d this is the 4 par (d) a int
add ecx,r8d						;add to ECX the dword at R8d this is the 3 par (c) a int
add ecx,edx						;add to ECX the dword at EDX this is the 2 par (b) a int
movd xmm0,ecx					;now move this addtion into XMM0 up this point all ints
cvtdq2pd xmm0,xmm0				;convert packed dwords integers to packed Double precision Floats
movlpd xmm1,realVal				;move DoubleWord(real8)realVal set to +1.5 to XMM1
addsd xmm1,qword ptr [rbp+38h]	;add to XMM1 the MMWORD PTR at [RSP+30h] this C++ double (f) a double
divsd xmm0,xmm1					;now div XMM0 with XMM1 storing the result into XMM0 this the value back to C
call restoreall
pop r15							;restore r15 correct stack
pop rbp							;restore the caller's rbp
ret								;DIVSD divide Scalar Double Precision Floating point Value
localdata qword 012345678h
CombineA ENDP
;*************************************************************************************
Public FunCpuid
FunCpuid proc
call storeall
mov eax,dword ptr temprcx			;Temprcx is par 1 (in this case was the eax value for cpuid ins)
cpuid								;do couid this will change regs eax,ebx,ecx and edx
mov tempeax,eax						;store the eax that cpuid ins gave us in (dword)tempeax 
mov tempebx,ebx						;store the ebx that cpuid ins gave us in (dword)tempebx 
mov tempecx,ecx						;store the ecx that cpuid ins gave us in (dword)tempecx 
mov tempedx,edx						;store the edx that cpuid ins gave us in (dword)tempedx 
mov ecx,tempeax						;now set ecx to par 1 
mov edx,tempebx						;now set edx to par 2 
mov r8d,tempecx						;now set r8d to par 3 
mov r9d,tempedx						;now set r9d to par 4 
sub rsp,20h							;setup shadow space for 4 Qwords 8 * 4 = 32 = 20h
call Showcpu						;now call the extern proc
add rsp,20h							;restore stack pointer
call restoreall
ret
FunCpuid endp
;*************************************************************************************
public GetSysDetails
GetSysDetails proc
mov rax,0
mov rdx,0
rdtsc
sub rsp,20h
call GetSystemInfo
add rsp,20h
mov al,0
mov rax,0
mov rdx,0
ret
GetSysDetails endp
;*************************************************************************************
public CheckBitSet
CheckBitSet proc
bt rcx,rdx
jnc notpresent
mov al,1
ret
notpresent:
mov al,0
ret

CheckBitSet endp
;*************************************************************************************
Public FuncGetFlags
FuncGetFlags proc
call storeall
mov r10,rcx
mov eax,dword ptr 1
cpuid													
bt edx,0												
jnc notedx0
mov byte ptr[r10.CPUFLAGS.fFpu],1		;bit0 = FPU
notedx0:
bt edx,23
jnc notedx23
mov byte ptr[r10.CPUFLAGS.fMmx],1		;bit23 = MMX
notedx23:
bt edx,25
jnc notedx25
mov byte ptr[r10.CPUFLAGS.fSse],1
notedx25:
bt edx,26
jnc notedx26
mov byte ptr[r10.CPUFLAGS.fSse2],1
notedx26:

bt ecx,0
jnc notecx0
mov byte ptr[r10.CPUFLAGS.fSse3],1
notecx0:
bt ecx,9
jnc notecx9
mov byte ptr[r10.CPUFLAGS.fSsse3],1
notecx9:
bt ecx,19
jnc notecx19
mov byte ptr[r10.CPUFLAGS.fSse4_1],1
notecx19:
bt ecx,20
jnc notecx20
mov byte ptr[r10.CPUFLAGS.fSse4_2],1
notecx20:
bt ecx,28
jnc notecx28
mov byte ptr[r10.CPUFLAGS.fAvx],1
notecx28:
bt ecx,12
jnc notecx12
mov byte ptr[r10.CPUFLAGS.fFma],1
notecx12:
bt ecx,25
jnc notecx25
mov byte ptr[r10.CPUFLAGS.fAesni],1
notecx25:
bt edx,18
jnc notedx18
mov byte ptr[r10.CPUFLAGS.fProsn],1
notedx18:
bt ecx,29
jnc notecx29
mov byte ptr[r10.CPUFLAGS.fF16c],1
notecx29:
bt ecx,30
jnc notecx30
mov byte ptr[r10.CPUFLAGS.fRand],1
notecx30:
call restoreall
ret
FuncGetFlags endp
;****************************************************************************
Public FuncPassPtr
FuncPassPtr proc
call storeall
mov rbx,Temprcx						;this is a Qword ptr from C++ to First par (regs[0] )
mov eax,1							;send 1 to regs[0] eax = 1
mov [rbx],eax						;now set regs[0] eax
add rbx,4							;mov ptr 4 bytes size of int
inc eax								;eax = eax + 1 eax = 2
mov [rbx],eax						;now send 2 to regs[1]
add rbx,4							;mov ptr 4 bytes size of int
inc eax								;eax = eax + 1 eax = 3
mov [rbx],eax						;now send 3 to regs[2]
add rbx,4							;mov ptr 4 bytes size of int
mov eax,1234h						;eax = eax + 1 eax = 4
mov [rbx],eax						;now send 4 to regs[3]
call restoreall
ret
FuncPassPtr endp
;*****************************************************************************
;FPU
Public FuncCpuPtr
FuncCpuPtr proc
call storeall
mov r10,Temprcx						;this is a Qword ptr from C++ to First par (regs[0] ) r10 points here
mov eax,[r10]						;regs[0] is used to set eax before cpuid
mov EntryEax,eax					;value used for eax before cpuid
cpuid
mov [r10],eax						;eax now set from Cpuid send to regs[0]
mov [r10 + 4],ebx					;ebx now set from Cpuid send to regs[1]
mov [r10 + 8],ecx					;ecx now set from Cpuid send to regs[2]
mov [r10 + 12],edx					;edx now set from Cpuid send to regs[3]
call restoreall

emms
fld real8 ptr[pfp1 + 8]
fadd real8 ptr[pfp5 + 16]
lea r8,bytes16
fbstp real10 ptr[bytes16]
mov eax,entryEax				;return eax that was sent to the cpuid
fld real8 ptr[fpone]
fldln2

ret
FuncCpuPtr endp
;**********************************************************************************
;SSE2
;enters with two doubles one in xmm0 first One(d3) and second in xmm1 (d1)
;returns (d2)in xmm0
public FuncXmm
FuncXmm proc
call storeall
minsd xmm0,xmm1						;xmm0 countains par 1 (d3) and xmm1 contains par 2 (d1) xmm0 now set to minimum
lea rbx,oldxmm0						;get address of oldmm into the rbx note define as xmmword ?
movdqa[rbx],xmm0					;mov the xmm0 into the xmmword this must be align 16 bytes see data section
push r9								;save r9 because malloc trashs the xmm0 and r9 maybe others
sub rsp,20h							;using 1 par rcx
mov rcx,1024						;rcx = amount of bytes to ask malloc for
call malloc							;destroys the xmm0 register?
pop r9								;restore R9
mov myMemory,rax					;rax = 64 bit pointer to it
add rsp,20h							;restore rsp
mov rbx,qword ptr myMemory			;place into the rbx the pointer given to us by malloc
mov rcx,8							;rcx counter place 1024 / 8 because there is 8 bytes in rax
mov rax,'BABABABA'					;place it our menory "BABABA"
Myloop:								;start of loop
mov [rbx],rax						;place the contends of the rax into [rbp]
add rbx,8							;move rbx along to next set of 8 bytes
loop Myloop							;dex rxc counter and if is not zero go round loop untill it is
sub rsp,20h							;we are about to call free with 1 par so making sure stack and place set
mov rcx,myMemory					;rcx is what free needs to free the memory given by malloc
call free							;now do it
add rsp,20h							;restore the rsp back
lea rbx,oldxmm0						;set the rbx to point to the xmm pointer to oldmm where xmm0 was stored
movdqa xmm0,xmmword ptr[rbx]
call restoreall
ret
FuncXmm endp

;**********************************************************************************
;enters with two sturtures one for cpu standard and one for extended
;returns 0 = false OR 1 = true
public FuncCpuAll
FuncCpuAll proc
call storeall
mov rax,0
cpuid
mov amountcpu,eax
mov eax,80000000h
cpuid
sub eax,80000000h
mov amountex,eax
mov Tempeax,eax
mov Tempebx,ebx
mov Tempecx,ecx
mov Tempedx,edx
mov rax,0
mov R10,Temprcx
mov R11,0
mov rcx,0
mov ecx,dword ptr amountcpu
inc ecx
Myloop:
push rcx
push rax
push r11
cpuid
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuedx,edx
pop r11
pop rax
pop rcx
add r11,16
inc rax
loopnz Myloop
mov rax,80000000h
mov R10,Temprdx
mov R11,0
mov rcx,0
mov ecx,dword ptr amountex
inc ecx
Myloop2:
push rcx
push rax
push r11
cpuid
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r10+r11].CpuData[0].Cpu.Cpuedx,edx
pop r11
pop rax
pop rcx
add r11,16
inc rax
loopnz Myloop2
call restoreall
mov rax,1							;true
ret
FuncCpuAll endp
;**********************************************************************************
;********************** FuncMemory *************************************************
;enters with The Address Of Memory Allocated From C++ With New Array Of 1024 Dword's
;C++ will also delete[] this memory;also using printf with double float and single float
;SSE2
public FuncMemory
FuncMemory proc
call storeall
push rsi
push rdi
lea rsi,mes1
mov rdi,Temprcx						;Address Of Memory From C++ (1024 DWORDs)
lea rbx,mes1
call GetStrLen						;on entry rbx points to string on exit rcx = lenght of string
MyMemloop:
mov al,byte ptr[rsi]
cmp al,0
je Loopstop
mov byte ptr[rdi],al
inc rsi
inc rdi
loopnz MyMemloop
loopstop:
mov byte ptr[rdi],al
pop rdi
pop rsi
sub rsp,38h							;shadow (32): + rcx (8): + Rdx  (8): + R8 (8) = 56 = 38h
lea rcx,mes2						;xmm0,xmm1 are returned in their registers no space needed on stack
movq xmm0,qword ptr[real_ld]		;displays dp float(real_ld) and sp float (real4_c)
movq rdx,xmm0
cvtss2sd xmm1,dword ptr[real4_d]	;now convert it to a dp float using floats (dp) in xmm0 and xmm1
movq r8,xmm1						;place the integer into next reg (Rcx,Rdx,R8,R9)
call printf
xor eax,eax
add rsp,38h
call restoreall
ret
FuncMemory endp
;**********************************************************************************
;returns a structure of CpuData type enters with ints 1,2,3,4
public Funcretinfo					;note caps are mixed only to show not needed
FuncRetInfo proc
call storeall
cmp rcx,1							;first par in rcx = flag
je CpuNormal						;if flag set then normal cpuid data needed
mov rax,80000000h					;else extended info needed so now call cpuid 
cpuid								;with eax = 80000000h will return max number in eax
sub eax,080000000h					;so by subtracting 80000000h we get amount of cpuid calls to make
mov dword ptr amountex,eax			;store this amount in the dword amountex

;CpuCell struc
;		Cpueax dword 0
;		Cpuebx dword 0
;		Cpuecx dword 0
;		Cpuedx dword 0
;CpuCell ends

;CpuData struc
;cpu CpuCell 32 dup ({0,0,0,0})
;CpuData ends

mov rcx,0							;clears bits above bit31 to zero's 
mov ecx,dword ptr amountex			;now rcx is set to max calls to cpuid
inc ecx
mov rax,0							;as for rcx above but for rax
mov eax,080000000h
Myloop2:
push rcx							;copy rcx to stack
push rax							;copy rax to stack
cpuid								;issuse cpuid with eax set 
mov dword ptr [r8].CpuData[0].Cpu.Cpueax,eax	;this is the data returned by cpuid
mov dword ptr [r8].CpuData[0].Cpu.Cpuebx,ebx	;this is the data returned by cpuid
mov dword ptr [r8].CpuData[0].Cpu.Cpuecx,ecx	;this is the data returned by cpuid
mov dword ptr [r8].CpuData[0].Cpu.Cpuedx,edx	;this is the data returned by cpuid
pop rax								;restore rax
pop rcx								;restore rcx *note the order first on last out
add r8,16							;mov r10 to next CpuCell (4 dwords = 4 *4) r10 + 16
inc rax								;mov to next cpuid to call
loopnz Myloop2						;this dec the rcx and if not zero jmps to Myloop2
jmp alldone_ext
CpuNormal:
mov eax,0
cpuid
mov amountcpu,eax

mov rax,0
mov rcx,0
mov ecx,dword ptr amountcpu
inc ecx
Myloop:
push rcx
push rax
cpuid
mov dword ptr [r8].CpuData[0].Cpu.Cpueax,eax
mov dword ptr [r8].CpuData[0].Cpu.Cpuebx,ebx
mov dword ptr [r8].CpuData[0].Cpu.Cpuecx,ecx
mov dword ptr [r8].CpuData[0].Cpu.Cpuedx,edx
pop rax
pop rcx
add r8,16
inc rax
loopnz Myloop				
call restoreall
mov ecx,dword ptr amountcpu
mov dword ptr[rdx], ecx
ret 
alldone_ext:
call restoreall
mov ecx,dword ptr amountex
mov dword ptr[rdx],ecx
ret 
Funcretinfo endp


;**********************************************************************************
;test function to test x87 and sse instruction return double precision float
;accepts XMM0 as first par and XMM1 as second
;SSE2,SSE3
FuncSums proc
call StoreAll
movd real_hd,xmm0
movhpd xmm4,real_hd
movd real_ld,xmm1
movlpd xmm4,real_ld
haddpd xmm4,xmm4
movdqa xmm0,xmm4
call RestoreAll
ret
FuncSums endp
;**********************************************************************************
;Creating a Thread in ASM 
public FuncThreadStart
FuncThreadStart proc
call StoreAll
mov rax,5000000
lea rdx,mutex
spinloop:
lock bts mutex,0				;this will inc mutex(Was 0) the carry is not set because note the lock
jc spinloop						;(All othet threads will be blocked untill its completes) it only went for 0 to 1 (No carry)
loophead:						;other threads will spinlock here untill first thread completes
inc dword ptr[rcx]
dec rax
jnz loophead
;call RestoreAll
mov mutex,0						;allow other threads to their work because mutex = zero so lock bts mutex,0 = No carry
ret
FuncThreadStart endp

;**********************************************************************************
;The Func that call win CreateThread **********************************************
public CreateThreadInAsm
CreateThreadInAsm proc
call StoreAll
push rbp	
mov rbp,rsp					;save callers rbp then set our rbp to the rsp
push 0							;On Stack 6th par for CreateThread threadid = None
push 0							;On Stack 5th par for CreateThread 0 = Creation Flag 
sub rsp,20h
mov r9,rcx						;R9 is Function(par) 4th par address in R9 
mov rcx,0						;RCX is par 1 Security attributes
mov rdx,0						;RDX is par 2th StackSize 
mov r8,FuncThreadStart			;R8 is par 3th par Function address
call CreateThread
mov rsp,rbp						;resets the rsp back to where it should be
pop rbp							;restore the rbp that was entered with
								;rax was the thread's handle					
;call RestoreAll					;return the thread handle to be used with WaitFor*** Object 
ret
CreateThreadInAsm endp

;**********************************************************************************
;test function to test x87 and sse instruction return double precision float
;accepts XMM0 as first par and XMM1 as second
;SSE,SSE2,SSE4.1
FuncFloats proc
call StoreAll
movups xmm3,xmmword ptr[rcx]
movups xmm4,xmmword ptr[rdx]
addps xmm3,xmm4
movups xmmword ptr[r8],xmm3
insertps xmm6,real4_a,00000000b
insertps xmm6,real4_b,00010000b
insertps xmm6,real4_c,00100000b
insertps xmm6,real4_d,00110000b
lea r10,bytes16							;or the quick way same result 
movdqu xmm1,[r10]
lea r10,avg16	
movdqu xmm0,[r10]
pavgb xmm0,xmm1
call RestoreAll
ret
FuncFloats endp
;************* The Followig Subs are used in the asm Func's ***********************
;copy values on entry from Registers to TempR*x also R8,9,10,11,12,13,14,15
public GetStrLen
GetStrLen proc
mov rcx,0								;maxium string len
loop1:
mov al,byte ptr[rbx]
cmp al,0
je loopok
inc rcx
inc rbx
cmp cl,MaxString
jb loop1
mov byte ptr[rbx],0
ret
loopok:
inc rcx
ret
GetStrLen endp

;********************************** StoreAll **************************************
;**********************************************************************************
;copy values on entry from Registers to TempR*x also R8,9,10,11,12,13,14,15
public StoreAll
StoreAll proc
mov Temprax,rax						
mov Temprbx,rbx			
mov Temprcx,rcx				
mov Temprdx,rdx					
mov TempR8,r8						
mov TempR9,r9						
mov TempR10,r10
mov TempR11,r11
mov TempR12,r12					
mov TempR13,r13						
mov TempR14,r14
mov TempR15,r15
ret
StoreAll endp
;**********************************************************************************
;******************************** RestoreAll **************************************
;copy values on entry to Registers from TempR*x also R8,9,10,11,12,13,14,15
public RestoreAll
RestoreAll proc
mov rax,Temprax						
mov rbx,Temprbx						
mov rcx,Temprcx						
mov rdx,Temprdx						
mov r8,TempR8						
mov r9,TempR9						
mov r10,TempR10
mov r11,TempR11
mov r12,TempR12					
mov r13,TempR13						
mov r14,TempR14
mov r15,TempR15
ret
RestoreAll endp
;**************************************************************************************

;****************************************************************************************
End
