


#define MAX_CONSOLEBUFFERS 2
extern "C"
{
	void		GetStartTimeStamp(ULARGE_INTEGER *);									//none
	void		GetEndTimeStamp(ULARGE_INTEGER *, ULARGE_INTEGER *, ULARGE_INTEGER *);	//none
}
class DosConsole
{

public:
	bool CpuidOk = false;
	int MaxBasicValue;
	int MaxExtendedValue;
	int CpuData[4];
	bool fAvxSupport;
	WCHAR wCpuInfo[64];
	DISPLAY display;
	int ActiveBuffers;
	char ConsoleTitleStd[256];
	HANDLE NewScreenBuffer[8];
	BOOL SetActiveBuffer(int Buffer);
	CONSOLE_CURSOR_INFO CursorInfoStd;	
	BOOLEAN StdFlag;
	HANDLE hOut;
	BOOLEAN StdInFlag;
	HANDLE hIn;
	DWORD LastError;
	DWORD AmountReadInput;
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER EndingTime;
	double ElapsedNanoseconds;
	double ElapsedMicroseconds;
	double ElapsedMilliseconds;
	double ElapsedSeconds;
	double CountPeriod;
	LARGE_INTEGER Frequency;
	LARGE_INTEGER LastDelay;

	ULARGE_INTEGER StartStampCounter;
	ULARGE_INTEGER EndStampCounter;
	ULARGE_INTEGER AmountStamps;
	ULARGE_INTEGER StampsFor1ms;

	struct _CPUFLAGS CpuFlags;
	struct _SYSTEM_INFO_DETAILS SysInfo;
	DosConsole();
	~DosConsole();
	UCHAR ShowDigits(DISPLAY *Display);
	CONSOLE_SCREEN_BUFFER_INFO GetBufferInfo();
	CONSOLE_SCREEN_BUFFER_INFO GetBufferInfoActive(int Buffer);

	BOOLEAN GetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCurorStd);
	BOOLEAN GetConsoleTitleStd(char *string);
	DWORD   GetMessageErrorString(DWORD ErrorNumber);
	bool	GetStartTime();
	bool	GetEndTime();
	bool    SetDelay(int delay);
	int		MakeScreenBuffer();
	BOOLEAN SetConsoleTitleStd(char *string);
	//bool	ShowTextCpu(int ineax, bool bflag);//struct _CPUDATA data);
	WORD    SetColorText(int TextColor);
	WORD    SetBackGroundText(int BackGroundColor);
	WORD    SetBackAndText(int TextColor ,int BackGroundColor);
	WORD    SetBackAndTextActive(int TextColor ,int BackGroundColor,int Buffer);
	BOOLEAN SetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCursorInfo);
	DWORD   SetScreenBackGroundActive(int BackGroundColor,int Buffer);
	DWORD   SetScreenBackGround(int BackGroundColor);
	BOOLEAN SetPos(int x,int y);
	BOOLEAN SetPosActive(int x,int y,int Buffer);

	BOOLEAN ReadInputRecordStd(INPUT_RECORD * InputRecord,BOOLEAN FlushFlag,DWORD *Read);
	BOOLEAN ReadConseleOutputAttribeStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read);
	
	BOOLEAN WriteConsoleOutputAttribsStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read);
}; 