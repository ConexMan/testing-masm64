#pragma once

#define MAX_CONSOLEBUFFERS 2
#pragma pack(push ,1)

typedef struct _CPUFLAGS
{
	BYTE fFpu;
	BYTE fMmx;
	BYTE fSse;
	BYTE fSse2;
	BYTE fSse3;
	BYTE fSsse3;
	BYTE fSse4_1;
	BYTE fSse4_2;
	BYTE fAvx;
	BYTE fFma;
	BYTE fAesni;
	BYTE fProsn;
	BYTE fF16c;
	BYTE fRand;
	BYTE spare4;
	BYTE spare5;
}CPUFLAGS, *LPCPUFLAGS;

typedef struct _SYSTEM_INFO_DETAILS
{
	DWORD dwOemId;
	DWORD dwPageSize;
	long long *lpMinAppAdress;
	long long *lpMaxAppAdress;
	long long *lpActiveProMask;
	DWORD dwAmountOfPros;
	DWORD dwProType;
	DWORD dwGran;
	WORD dwProcLv1;
	WORD dwProcRev;
}SYSTEM_INFO_DETAILS, *LPSYSTEM_INFO_DETAILS;

#pragma pack (pop)

//********************************
typedef struct _DISPLAY
{
	ULARGE_INTEGER Number;
	BYTE AmountSet;
	WCHAR Bufferset[128];
	WCHAR formatbuffer[64];
	BYTE numberformat;
	BYTE Digits[32];
}DISPLAY, *LPDISPLAY;
