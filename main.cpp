#define _CRT_RAND_S

#define _CRT_RAND_S
#define NONE		0
#define MULTIPLY	1
#define DIVIDE		2
#define ADD			3
#define SUBTRACT	4
#define MAXIMUM		5
#define MINIMUM		6
#define SQROOT		7
#define TESTING		0xFF

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <Strsafe.h>
#include <process.h>  

#include <emmintrin.h>

#include <stdio.h>
#include <limits.h>

#include "global.h"
#include "classdosheader.h"
#include "definecolors.h"
#include "classdoscode.cpp"
//#include "Aes.h"

	using namespace std;

	class DosConsole dos;
	bool CheckBitSetC(LARGE_INTEGER data, BYTE bitpos);

	bool ShowTextCpu(int ineax, bool bExtended, struct _CpuData *Cells);

	double CombineC(int a, int b, int c, int d, int e, double f)
	{
		double answer = 0;

		return double( a + b + c + d + e) / (f);

	}
	
#pragma pack(push ,1)
	typedef struct _CpuCell
	{
		int eax;
		int ebx;
		int ecx;
		int edx;

	}CpuCell;
	__declspec (align (16))

		typedef struct _CpuData
	{
		struct _CpuCell Cpu[32];
	}CpuData,*LPCpuData;

	
#pragma pack (pop)

	__declspec(align(32)) typedef struct _MULTIPLY_FPDOUBLE
	{
		long long  AmountOfDoubles;
		long long  ArithmeticType;
		long long  ActionOnType;
		long long  SpecicalAction;
		double InputBufferOne[4];
		double InputBufferTwo[4];
		double OutPutBuffer[4];
	}MULTIPLY_FPDOUBLE,*LPMULTIPLY_FPDOUBLE;

	

	// NOTE: extern “C” needed to prevent C++ name mangling
	extern "C"
	{
		double		CombineA(int a, int b, int c, int d, int e, double f);					//sse2
		void		FunCpuid(int cpueax, int cpuebx, int cpuecx,int cpuedx);				//none
		void		FuncPassPtr(int *);														//none
		int 		FuncCpuPtr(int *,double *);												//fpu
		double		FuncXmm(double f1, double d1);											//sse2
		int			FuncCpuAll(struct _CpuData *Standard,struct _CpuData *Extended);		//none
		void		FuncMemory(long long *address);											//sse2
		double		FuncSums(double a, double b);											//sse2,sse3
		float		FuncFloats(float *af, float *bf, float *cf);							//sse,sse2,sse4.1
		HANDLE		CreateThreadInAsm(int *i);												//none dont use possible problem
		void		FuncThreadStart(int *i);												//none dont use possible problem
		void        FuncAvx(double *dp_in, double *dp_out, float *fp_in, float *fp_out);	//fvx
		void		FuncRetInfo(int flag, int *len,struct _CpuData *);						//none
		double		FuncArithmetic(struct _MULTIPLY_FPDOUBLE *array);						//sse,sse2,sse3
		void		FuncGetFlags(struct _CPUFLAGS *);										//none
		BYTE        CheckBitSet(LARGE_INTEGER Data, BYTE bitpos);							//none
		bool        GetSysDetails(struct _SYSTEM_INFO_DETAILS *SystemDetails);				//none
		void		GetStartTimeStamp(ULARGE_INTEGER *);									//none
		void		GetEndTimeStamp(ULARGE_INTEGER *,ULARGE_INTEGER *,ULARGE_INTEGER *);	//none
		BOOL		GetTicksForOneMs(ULARGE_INTEGER *);										//sse2

	}

	struct _CpuData Standard;
	struct _CpuData	Extended;
	//struct _CPUFLAGS Flags

	extern "C" void Showcpu(int a, int b, int c, int d)
	{
		static int scount = 2;
			Extended.Cpu[scount].eax = a;
			Extended.Cpu[scount].ebx = b;
			Extended.Cpu[scount].ecx = c;
			Extended.Cpu[scount].edx = d;		
			scount++;
			if (scount == 4)
			{
				ShowTextCpu(2, true, &Extended);
//				printf("%S", dos.wCpuInfo);
				printf("\n\n");
			}
	}
	int main(void)
	{
		LARGE_INTEGER Data;
		WORD OldAttrib;
		HANDLE ThreadHandle = 0;
		int ret;
		float *af, *bf,*cf;
		double *doublesIn, *doublesOut;
		float *floatsIn, *floatsOut;
		int count;
		int cpueax	= 0x80000002;

		int cpuebx	= 0;
		int cpuecx	= 0;
		int cpuedx	= 0;
		int i = 0;
		int *regs;
		long long *mem;
		int oldeax;
		double answer;
		DWORD AmountEx;
		bool favx		= false;
		bool fsse3		= false;
		bool fssse3		= false;
		bool fsse4_1	= false;
		bool fsse4_2	= false;
		bool fsse2		= false;
		bool fsse		= false;
		bool fmmx		= false;
		BYTE dataset = 0;
		BYTE bitpos;
		unsigned int	number = 0;
		double			number1 = 0;
		double          max = 100.0;
		//errno_t         err;
		double d1, d2, d3;
		double MinPeriod = 0.0;
		struct _CpuData Standard;
		struct _CpuData	Extended;
		struct _MULTIPLY_FPDOUBLE Multiply1;
		struct _CPUFLAGS Flags = { 0 };
		double a, b, c;
		double fpdoubles[4];
		BOOL Status = false;
		int flag = 0;
		int Len = 0;
		CpuData *cpudata;
		CpuData *cpudataex;
	
		answer = 1.0f;
		dos.SetScreenBackGround(BLUE);
		OldAttrib = dos.SetBackAndText(WHITE, BLUE);
		Status = GetTicksForOneMs(&dos.StampsFor1ms);
		if(Status != 1)
		printf("The Value returned from GetTicksForOneMs was  %I64u\n", dos.StampsFor1ms.QuadPart);
		else
			printf("The Masm Function GetTicksForOneMs Failed!\n");
		
		
		regs		= new int[sizeof(int) * 4];
		//******************** Allocate memory with New *******************
		doublesIn	= new double[sizeof(double) * 8];
		doublesOut	= new double[sizeof(double) * 8];
		floatsIn	= new float[sizeof(float) * 16];
		floatsOut	= new float[sizeof(float) * 16];
	

		mem			= new long long[sizeof(DWORD) * 1024];
		af			= new float[sizeof(float) * 4];
		bf			= new float[sizeof(float) * 4];
		cf			= new float[sizeof(float) * 4];	
		cpudata		= new CpuData[(sizeof(CpuData))];
		cpudataex	= new CpuData[(sizeof(CpuData))];
		//*******************************************************************
		Multiply1.AmountOfDoubles	= 10;
		Multiply1.InputBufferOne[0] = 10.8505600;
		Multiply1.InputBufferOne[1] = 1067.8567000;
		Multiply1.InputBufferOne[2] = 1.85123000;
		Multiply1.InputBufferOne[3] = -500.8506700;		
		Multiply1.InputBufferTwo[0] = 8.1;
		Multiply1.InputBufferTwo[1] = 10.780;
		Multiply1.InputBufferTwo[2] = 5000.89;
		Multiply1.InputBufferTwo[3] = 3000.78;
		Multiply1.OutPutBuffer[0]	= 0.0;
		Multiply1.OutPutBuffer[1]	= 0.0;
		Multiply1.OutPutBuffer[2]	= 0.0;
		Multiply1.OutPutBuffer[3]	= 0.0;

		
		SecureZeroMemory(&dos.SysInfo, sizeof(SYSTEM_INFO_DETAILS));
		
		GetSysDetails(&dos.SysInfo);
		printf("The Amount Of Processors Is %d\n", dos.SysInfo.dwAmountOfPros);
		printf("The Max address Is %pX\n", dos.SysInfo.lpMaxAppAdress);
		//*********************************************
		Data.QuadPart = 0x8000000000000000 ;
		bitpos = 63;
		dos.GetStartTime();
		dataset = CheckBitSet((LARGE_INTEGER)Data,bitpos);
		dos.GetEndTime();
		dataset = dataset & 1;
		if (dataset == 1)
			printf("BitSet (Masm) %u is Set in Data\n", bitpos);
		else if (dataset != 1)
			printf("BitSet (Masm) %u is Not Set in Data\n", bitpos);
		printf("The masm GetBitSet() took %8.8f ns\n", dos.ElapsedNanoseconds);
		//**************************************************
	
		Data.QuadPart = 0x8000000000000000;
		bitpos = 63;
		dos.GetStartTime();
		CheckBitSetC((LARGE_INTEGER)Data, bitpos);
		dos.GetEndTime();
		dataset = dataset & 1;
		if (dataset == 1)
			printf("BitSet (C++) %u is Set in Data\n", bitpos);
		else if (dataset != 1)
			printf("BitSet (C++) %u is Not Set in Data\n", bitpos);
		printf("The C++ GetBitSetC() took %8.8f ns\n", dos.ElapsedNanoseconds);
		//***************************************************
	
		dos.GetStartTime();
		GetStartTimeStamp(&dos.StartStampCounter);
		FuncGetFlags(&dos.CpuFlags);//no sse needed
		GetEndTimeStamp(&dos.EndStampCounter, &dos.AmountStamps, &dos.StartStampCounter);
		dos.GetEndTime();
		printf("The Masm FuncGetFlags() took %8.8f ns\n", dos.ElapsedNanoseconds);
		printf("The Amount Of Time stamps for FuncGetFlags() Was %I64u\n", dos.AmountStamps.QuadPart);

		if(dos.CpuFlags.fFpu == 1)
			printf("The Cpu Supports FPU			Instructions\n");
		else 
			printf("The Cpu does not Supports FPU Instructions\n");

		if (dos.CpuFlags.fMmx == 1)
			printf("The Cpu Supports MMX			Instructions\n");
		else
			printf("The Cpu does not Supports MMX Instructions\n");

		if (dos.CpuFlags.fSse == 1)
			printf("The Cpu Supports SSE			Instructions\n");
		else
			printf("The Cpu does not Supports			SSE Instructions\n");
		if (dos.CpuFlags.fSse2 == 1)
			printf("The Cpu Supports SSE2			Instructions\n");
		else
			printf("The Cpu does not Supports			SSE2 Instructions\n");
		if (dos.CpuFlags.fSse3 == 1)
			printf("The Cpu Supports SSE3			Instructions\n");
		else
			printf("The Cpu does not Supports			SSE3 Instructions\n");
		if (dos.CpuFlags.fSsse3 == 1)
			printf("The Cpu Supports SSSE3			Instructions\n");
		else
			printf("The Cpu does not Supports SSSE3			 Instructions\n");
		if (dos.CpuFlags.fSse4_1 == 1)
			printf("The Cpu Supports SSE4.1			Instructions\n");
		else
			printf("The Cpu does not Supports			SSE4.1 Instructions\n");
		if (dos.CpuFlags.fSse4_2 == 1)
			printf("The Cpu Supports SSE4.2			Instructions\n");
		else
			printf("The Cpu does not Supports SSE4.2	Instructions\n");
		if (dos.CpuFlags.fAvx == 1)
			printf("The Cpu Supports AVX			Instructions\n");
		else
			printf("The Cpu does not Supports AVX		Instructions\n");
		if (dos.CpuFlags.fFma == 1)
			printf("The Cpu Supports FMA			Instructions\n");
		else
			printf("The Cpu does not Supports FMA		Instructions\n");
		if (dos.CpuFlags.fAesni == 1)
			printf("The Cpu Supports AESNI			Instructions\n");
		else
			printf("The Cpu does not Supports AESNI		Instructions\n");
		if (dos.CpuFlags.fProsn == 1)
			printf("The Cpu Supports Pro S/N			Instructions\n");
		else
			printf("The Cpu does not Supports Pro S/N	Instructions\n");

		if (dos.CpuFlags.fF16c == 1)
			printf("The Cpu Supports 16 FP(c)		Instructions\n");
		else
			printf("The Cpu does not Supports 16 FP(c)	Instructions\n");
		if (dos.CpuFlags.fRand == 1)
			printf("The Cpu Supports Rand			Instructions\n");
		else
			printf("The Cpu does not Supports Rand		Instructions\n");
		printf("//********************************************************\n\n");
		doublesIn[0] = 2.5;
		doublesIn[1] = 4.0;
		doublesIn[2] = 6.0;
		doublesIn[3] = 10.0;
		doublesIn[4] = 25.0;
		doublesIn[5] = 35.0;
		doublesIn[6] = 50.0;
		doublesIn[7] = 70.0;

		floatsIn[0] = 1.25f;
		floatsIn[1] = -100.78f;
		floatsIn[2] = 11.25f;
		floatsIn[3] = 10.718f;
		floatsIn[4] = -16.25f;
		floatsIn[5] = 1200.78f;
		floatsIn[6] = 0.25f;
		floatsIn[7] = -23.78f;

		floatsIn[8] = 100.25f;
		floatsIn[9] = -110.78f;
		floatsIn[10] = 35.25f;
		floatsIn[11] = 34.718f;
		floatsIn[12] = -0.25f;
		floatsIn[13] = 200.78f;
		floatsIn[14] = 67.25f;
		floatsIn[15] = -245.78f;




		floatsOut[0] = 0.0f;
		floatsOut[1] = 0.0f;
		floatsOut[2] = 0.0f;
		floatsOut[3] = 0.0f;
		floatsOut[4] = 0.0f;
		floatsOut[5] = 0.0f;
		floatsOut[6] = 0.0f;
		floatsOut[7] = 0.0f;

		if (dos.CpuFlags.fAvx)//avx needed
		{
			dos.GetStartTime();
		
			FuncAvx(doublesIn, doublesOut, floatsIn, floatsOut);
			printf("doublesOut 0 = %8.4f	1 = %8.4f	2 = %8.4f	3 = %8.4f\n", doublesOut[0],
				doublesOut[1], doublesOut[2], doublesOut[3]);
			printf("doublesOut 4 = %8.4f	5 = %8.4f	6 = %8.4f	7 = %8.4f\n", doublesOut[4],
				doublesOut[5], doublesOut[6], doublesOut[7]);

			printf("floatsOut 0 = %8.4f	1 = %8.4f	2 = %8.4f	3 = %8.4f\n", floatsOut[0],
				floatsOut[1], floatsOut[2], floatsOut[3]);
			printf("floatsOut 4 = %8.4f	5 = %8.4f	6 = %8.4f	7 = %8.4f\n", floatsOut[4],
				floatsOut[5], floatsOut[6], floatsOut[7]);
			printf("floatsOut 8 = %8.4f	9 = %8.4f	10 = %8.4f	11 = %8.4f\n", floatsOut[8],
				floatsOut[9], floatsOut[10], floatsOut[11]);
			printf("floatsOut 12 = %8.4f	13 = %8.4f	14 = %8.4f	15 = %8.4f\n", floatsOut[12],
				floatsOut[13], floatsOut[14], floatsOut[15]);
			
			dos.GetEndTime();
			printf("The masm FuncAvx took %8.4f us\n", dos.ElapsedMicroseconds);
			
			printf("\n\n");
		}
		
		dos.GetStartTime();
		
		printf("Amount Of Double From FuncArithmetic is Now %d\n", (unsigned int)Multiply1.AmountOfDoubles);
		printf("C++ Answer = %8.8f ", Multiply1.InputBufferOne[0] * Multiply1.InputBufferTwo[0]);
		printf("C++ Answer = %8.8f ", Multiply1.InputBufferOne[1] * Multiply1.InputBufferTwo[1]);

		printf("C++ Answer = %8.8f ", Multiply1.InputBufferOne[2] * Multiply1.InputBufferTwo[2]);
		printf("C++ Answer = %8.8f ", Multiply1.InputBufferOne[3] * Multiply1.InputBufferTwo[3]);
		printf("\n");
		
		dos.GetEndTime();
		printf("The Amount Of Time stamps for Func() Was %I64u\n", dos.AmountStamps.QuadPart);
		printf("The C++ Program Took %8.4f us\n", dos.ElapsedMicroseconds);

		dos.GetStartTime();
		GetStartTimeStamp(&dos.StartStampCounter);
		Multiply1.ArithmeticType = TESTING;//needs sse,sse2,sse3
		answer = FuncArithmetic(&Multiply1);
		GetEndTimeStamp(&dos.EndStampCounter, &dos.AmountStamps, &dos.StartStampCounter);
		dos.GetEndTime();
		printf("The Amount Of Time stamps Was for FuncArithmetic() %I64u\n", dos.AmountStamps.QuadPart);

		printf("Amount Of Double From FuncMultiplyFpDoubles is Now %d\n",(unsigned int)Multiply1.AmountOfDoubles);
		printf("Answer 0 = %8.10f Answer 1 = %8.10f\n", Multiply1.OutPutBuffer[0], Multiply1.OutPutBuffer[1]);
		printf("Answer 2 = %8.10f Answer 3 = %8.10f\n", Multiply1.OutPutBuffer[2], Multiply1.OutPutBuffer[3]);
		printf("The answer from FuncArithmetic was %8.8f\n", answer);
		
		printf("The ASM Program FuncArithmetic Took %8.4f us\n", dos.ElapsedMicroseconds);

		//regs[0] = 0x80000000;
		printf("The Value Of i Is now %d\n", i);
	
		dos.GetStartTime();
		
		ThreadHandle = CreateThreadInAsm(&i);
		
		FuncThreadStart(&i);
		dos.GetStartTime();
		WaitForSingleObject(ThreadHandle, 1000); //waiting max 100ms
		dos.GetEndTime();
		printf("The WaitForSingleOject waited  %8.4f us\n", dos.ElapsedMicroseconds);
	
		printf("hThread was %llX\n", (unsigned long long)ThreadHandle);
		printf("The Value Of i Is now %d\n", i);
		dos.GetEndTime();
		printf("The ASM Program CreateThreadInAsm Took %8.6f ms\n", dos.ElapsedMilliseconds);
		
		//************************** Timing for The C++ Prpgram *************************
		dos.GetStartTime();
		printf("CombineC (C++ Proc Returned) %8.4f\n", CombineC(1, 2, 3, 4, 5, 6.1));//c++ program
		dos.GetEndTime();
		printf("The C++ Program Took %8.4f us\n", dos.ElapsedMicroseconds);
		//********************************************************************************
		//************************** Timing for the ASM Program **************************
		dos.GetStartTime();

		printf("CombineA (asm Proc Returned) %8.16f\n", CombineA(1, 2, 3, 4, 5, 6.1));//needs sse2
		dos.GetEndTime();
		printf("The ASM Program Took %8.4f us\n", dos.ElapsedMicroseconds);
		//*********************************************************************************
		for (count = 0; count < 3; count++)
		{
			FunCpuid(cpueax, cpuebx, cpuecx, cpuedx);//none
			cpueax++;
		}

		FuncPassPtr(regs);//none
		printf("From FuncPassPtr() eax = %X, ebx = %X, ecx = %X, edx = %X \n", regs[0], regs[1], regs[2], regs[3]);
		regs[0] = 1;
		oldeax	= 0;
		fpdoubles[0] = 1.0f;
		fpdoubles[1] = 2.0f;
		fpdoubles[2] = 3.0f;
		fpdoubles[3] = 4.0f;
		oldeax = FuncCpuPtr(regs,fpdoubles);//none
		printf("From FuncCpuPtr() eax = %X, ebx = %X, ecx = %X, edx = %X from a87 = %d\n", regs[0], regs[1], regs[2], regs[3],oldeax);
		
	
			printf("Double 0 = %6.4f, Double 1 = %6.4f Double 2 = %6.4f Double 3 = %6.4f \n",
				fpdoubles[0],fpdoubles[1],fpdoubles[2],fpdoubles[3]);

		d2 = 0.0;
		d1 = -513.68;
		d3 = -660.57;

		dos.GetStartTime();
		d2 = FuncXmm(d3, d1);//sse2
		dos.GetEndTime();
		printf("The ASM FuncXmm Took %8.4f us\n", dos.ElapsedMicroseconds);

		printf("FuncXmm() Min of d3 and d1 %8.4f \n", d2);
	
		Standard.Cpu[0].eax = 0xff;
		dos.GetStartTime();
		ret = FuncCpuAll(&Standard, &Extended);//none
		dos.GetEndTime();
		printf("The ASM FuncCpuAll Took %8.4f us\n", dos.ElapsedMicroseconds);
		dos.MaxBasicValue = Standard.Cpu[0].eax;
		dos.MaxExtendedValue = Extended.Cpu[0].eax;
		printf("\n\n");
		
		
		printf("******************************** Standard Cpuid ***********************\n");
		for(count = 0; count <= Standard.Cpu[0].eax;count++)
		{
			printf("FuncCpuAll Offset = %d and eax = %X  ebx = %X  ecx = %X  edx = %X \n",
				count, Standard.Cpu[count].eax, Standard.Cpu[count].ebx, Standard.Cpu[count].ecx, Standard.Cpu[count].edx);
		}

		AmountEx = 0;
		AmountEx = Extended.Cpu[0].eax - 0x80000000;
		ShowTextCpu(0, false, &Standard);
		printf("%S\n", dos.wCpuInfo);
		printf("Max Eax To Use With CPUID Is %d\n", dos.MaxBasicValue);
		printf("*********************************************************************\n");
		printf("\n");
		printf("******************************** Extended Cpuid ***********************\n");
		for (count = 0; count <= 1; count++)
		{
			printf("FuncCpuAll Offset = %d and eax = %X ebx = %X ecx = %X edx = %X \n",
				count, Extended.Cpu[count].eax, Extended.Cpu[count].ebx, Extended.Cpu[count].ecx, Extended.Cpu[count].edx);
		
		}
		
		ShowTextCpu(2, true, &Extended);
		printf("%S\n", dos.wCpuInfo);
		printf("Max Extended value for CPUID Is %8.8X \n", dos.MaxExtendedValue);
		printf("***********************************************************************\n");
		
		*mem = 0x1234;
		strcpy_s((char *)mem, 1024 * 4, "Hello Brian From C++");

		printf("The Contents of before FuncMemory %s \n",(char *) mem);
		FuncMemory(mem);//sse2
		printf("The Contents of after  FuncMemory %s \n",(char *) mem);

		//******************************* OK to here *************************
		a = 0.0;
		b = 9023.05679;
		c = 1287.0409509;
		dos.GetStartTime();
		a = FuncSums(b, c);//sse2,sse3
		dos.GetEndTime();
		printf("The ASM FuncSums Took %8.10f us\n", dos.ElapsedMicroseconds);
		printf("The Hypp is %4.4f\n", a);
		af[0] = 100.575f;
		af[1] = 300.564f;
		af[2] = 4556.866f;
		af[3] = 6.9666f;

		bf[0] = 710.5979f;
		bf[1] = 130.52313f;
		bf[2] = 545.8546f;
		bf[3] = 766.9867f;

		cf[0] = 0.0f;
		cf[1] = 0.0f;
		cf[2] = 0.0f;
		cf[3] = 0.0f;
		
		dos.GetStartTime();
		if (dos.CpuFlags.fSse4_1)
		{
			answer = FuncFloats(af, bf, cf);//sse,sse2,sse4.1
			dos.GetEndTime();
			printf("The ASM FuncFloats Took %8.4f us\n", dos.ElapsedMicroseconds);
			printf("Floats cf[0] = %6.4f cf[1] = %6.4f cf[2] = %6.4f cf[3] = %6.4f Returned with %8.4f \n",
				cf[0], cf[1], cf[2], cf[3], answer);
		}
		else
			printf("This processor does not contain sse4.1 Instructions\n");
		//**************************************************************************************************
		flag = 1;

	    FuncRetInfo(flag,&Len,cpudata);//none
		for (count = 0; count <= Len; count++)
		{
			printf("FuncRetInfo Offset = Eax %X  Ebx %X Ecx %X Edx %X \n", cpudata->Cpu[count].eax, cpudata->Cpu[count].ebx,
				cpudata->Cpu[count].ecx, cpudata->Cpu[count].edx);
		}
		ShowTextCpu(0, false, cpudata);
		printf("Maxium Value of eax in cpuid is %d\n", cpudata->Cpu->eax);
		printf("%S\n", dos.wCpuInfo);
		printf("\n\n");
		
		flag = 0;

		FuncRetInfo(flag, &Len,cpudataex);//none
		for (count = 0; count <= Len; count++)
		{
			printf("FuncRetInfo Offset = Eax %X  Ebx %X Ecx %X Edx %X \n", cpudataex->Cpu[count].eax, cpudataex->Cpu[count].ebx,
				cpudataex->Cpu[count].ecx, cpudataex->Cpu[count].edx);
		}
		ShowTextCpu(2, true, cpudataex);
		printf("Maxium Value of eax in cpuid is %X\n", cpudataex->Cpu->eax);
		printf("%S\n", dos.wCpuInfo);
	
		printf("\n\n");
		
		//************************************************************************************************
		if (ThreadHandle != NULL)
			CloseHandle(ThreadHandle);
		getchar();

		delete[] regs;
		delete[] mem;
		delete[] af;
		delete[] bf;
		delete[] cf;
		delete[] cpudata;
		delete[] cpudataex;
		delete[] doublesIn;
		delete[] doublesOut;
		delete[] floatsIn;
		delete[] floatsOut;
		return 0;

	}
	//************************************ 
	bool ShowTextCpu(int ineax, bool bExtended, CpuData *Cells)
	{
	int count;
	int index;
	int count1;
	char cStrings[64];
	WORD wtemp;

	if ((ineax == 0) && (!bExtended))//do not use EAX for chars copy EBX,EDX,ECX in this order
	{
	index = 0;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx

	wtemp = LOWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = LOWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//add a space
	cStrings[index] = (char)LOBYTE(' ');
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ecx
	wtemp = LOWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = LOWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//14 terminator
	cStrings[index] = 0;
	MultiByteToWideChar(CP_ACP, 0, cStrings, sizeof(cStrings), dos.wCpuInfo, _countof(dos.wCpuInfo));
	}
	index = 0;
	if ((ineax == 2) && (bExtended))
	{
	for (count1 = 0; count1 < 3; count1++)
	{
	//Eax
	wtemp = LOWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Eax
	wtemp = LOWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Eax
	wtemp = HIWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Eax
	wtemp = HIWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ebx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ecx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Edx
	wtemp = LOWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = LOWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	}
	//terminator
	cStrings[index] = 0;
	MultiByteToWideChar(CP_ACP, 0, cStrings, sizeof(cStrings), dos.wCpuInfo, _countof(dos.wCpuInfo));
	}
	count = 0;
	return true;
	}
	//***************************************
	bool CheckBitSetC(LARGE_INTEGER data, BYTE bitpos)

	{
		data.QuadPart = data.QuadPart >> bitpos;
		data.QuadPart = data.QuadPart &1;
		if (data.QuadPart == 1)
			return true;
		else
			return false;

	}
