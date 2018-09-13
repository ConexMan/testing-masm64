
DosConsole::DosConsole(void)
{
//	printf("In Class Constructor \n");
	DosConsole::hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if(hOut == INVALID_HANDLE_VALUE )
		DosConsole::StdFlag = FALSE;
	else
	DosConsole::StdFlag   = TRUE;
	DosConsole::hIn = GetStdHandle(STD_INPUT_HANDLE);
	if(hIn == INVALID_HANDLE_VALUE )
		DosConsole::StdInFlag = FALSE;
	else
	DosConsole::StdInFlag   = TRUE;
	DosConsole::ActiveBuffers = 0;
	QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency.QuadPart);
	__cpuidex(CpuData, 1, 0);
	if ((CpuData[02] >> 28 & 1) == 1)
	{
		printf("Cpu supports AVX instructions\n");
		fAvxSupport = true;
	}
	else
	{
		fAvxSupport = false;
		printf("Cpu does not supports AVX instructions\n");
	}
}

//****************************************

DosConsole::~DosConsole(void)
{
	int count;
//	printf("In Class DeConstructor \n");
	if(DosConsole::StdFlag)
	CloseHandle(DosConsole::hOut);
	if(DosConsole::StdInFlag)
	CloseHandle(DosConsole::hIn);
	if((DosConsole::ActiveBuffers) && (DosConsole::ActiveBuffers < 8))
	{
		for(count = 0;count < DosConsole::ActiveBuffers;count++)
		{
			CloseHandle(DosConsole::NewScreenBuffer[count]);
		}
	}

}

//***************************************
UCHAR DosConsole::ShowDigits(DISPLAY *Display)
{
	int count;
	int digitszero;

	for (count = 0; count < Display->AmountSet; count++)
	{
		Display->Digits[count] = Display->Number.QuadPart % 10;
		Display->Number.QuadPart = Display->Number.QuadPart / 10;
	}
	Display->AmountSet = count;
	digitszero = 0;
	for (count = Display->AmountSet; count > 0; count--)
	{
		if (Display->Digits[count - 1] == 0)
			digitszero++;
		else
			break;
	}

	switch (digitszero)
	{
	case 0:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[19],Display->Digits[18], Display->Digits[17], Display->Digits[16], Display->Digits[15], Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 1:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[18], Display->Digits[17], Display->Digits[16], Display->Digits[15], Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 2:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[17], Display->Digits[16], Display->Digits[15],Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 3:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[16], Display->Digits[15], Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 4:
		StringCchPrintf((STRSAFE_LPSTR)(STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[15], Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 5:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
		    Display->Digits[14], Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 6:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[13],
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 7:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[12], Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5], 
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 8:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[11], Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 9:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[10], Display->Digits[9],
			Display->Digits[8], Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 10:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[9], Display->Digits[8], Display->Digits[7], Display->Digits[6],
			Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 11:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d,%d%d%d,%d%d%d",
			Display->Digits[8], Display->Digits[7], Display->Digits[6],
			Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 12:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d,%d%d%d",
			Display->Digits[7], Display->Digits[6], Display->Digits[5],
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 13:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d,%d%d%d",
			Display->Digits[6], Display->Digits[5], Display->Digits[4],
			Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 14:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d,%d%d%d",
			Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 15:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d,%d%d%d",
			Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 16:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d,%d%d%d",
			Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 17:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d%d", Display->Digits[2], Display->Digits[1], Display->Digits[0]);
		break;
	case 18:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d%d", Display->Digits[1], Display->Digits[0]);
		break;
	case 19:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d", Display->Digits[0]);
		break;
	case 20:
		StringCchPrintf((STRSAFE_LPSTR)Display->Bufferset, _countof(Display->Bufferset), (STRSAFE_LPSTR)L"%d", 0);
		break;
	}
	wcscat_s(Display->Bufferset, _countof(Display->Bufferset), Display->formatbuffer);

	return 1;
}

bool DosConsole::SetDelay(int delay)
{
	BOOL result = FALSE;
	LARGE_INTEGER WaitTime = { 0 };
	double waitperiod = 0.0;
	double waitperiod1 = 0.0;
	if (Frequency.LowPart == 0)
	{
		result = QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency.QuadPart);
		if (!result)
			return false;
	}
	waitperiod = (double) 1 / Frequency.LowPart;//amount of ticks per 1ms
	waitperiod1 = 0.001 / waitperiod;
	
	
	GetStartTime();

	WaitTime.QuadPart = StartingTime.QuadPart + (long long)waitperiod1;
	GetStartTimeStamp(&StartStampCounter);
	do
		GetStartTime();
	while (StartingTime.QuadPart < WaitTime.QuadPart);
	GetEndTimeStamp(&EndStampCounter, &AmountStamps, &StartStampCounter);
	GetEndTime();
	StampsFor1ms.QuadPart = AmountStamps.QuadPart;
	return true;
}

WORD DosConsole::SetColorText(int TextColor)
{
	BOOL ScreenFlag,SetTextFlag;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	if(DosConsole::StdFlag)//ok
	{
	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::hOut,&ScreenBuffer);
	SetTextFlag	= SetConsoleTextAttribute(DosConsole::hOut,TextColor);
		return (WORD) ScreenBuffer.wAttributes;
	}
	else//no handle!
		return 0;
}

//***************************************

WORD DosConsole::SetBackGroundText(int BackGroundColor)
{
	BOOL ScreenFlag,SetTextFlag;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	BackGroundColor = BackGroundColor << 4;
	if(DosConsole::StdFlag)//ok
	{
	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::hOut,&ScreenBuffer);
	SetTextFlag	= SetConsoleTextAttribute(DosConsole::hOut,BackGroundColor);
		return (WORD) ScreenBuffer.wAttributes;
	}
	else//no handle!
		return 0;
}

//***************************************

WORD DosConsole::SetBackAndText(int TextColor ,int BackGroundColor)
{
	BOOL ScreenFlag,SetTextFlag;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	BackGroundColor = BackGroundColor << 4;
	TextColor       = TextColor + BackGroundColor;
	if(DosConsole::StdFlag)//ok
	{
	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::hOut,&ScreenBuffer);
	SetTextFlag	= SetConsoleTextAttribute(DosConsole::hOut,TextColor);
		return (WORD) ScreenBuffer.wAttributes;
	}
	else//no handle!
		return 0;
}
//***************************************

DWORD DosConsole::SetScreenBackGround(int BackGroundColor)
{
	BOOL ScreenFlag,SetFillFlag;
	DWORD Written;
	DWORD MaxCells;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	COORD cordpos;
	BackGroundColor = BackGroundColor << 4;
	cordpos.X = 0;
	cordpos.Y = 0;

	if(DosConsole::StdFlag)//ok
	{
	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::hOut,&ScreenBuffer);	
	MaxCells	= ScreenBuffer.dwMaximumWindowSize.X * ScreenBuffer.dwMaximumWindowSize.Y;
	SetFillFlag = FillConsoleOutputAttribute(DosConsole::hOut,BackGroundColor,MaxCells,cordpos,&Written);
		return DWORD(Written);
	}
	else//no handle!
		return 0;
}
//********************************************

BOOLEAN DosConsole::SetPos(int x,int y)
{
	COORD pos = {(SHORT)x, (SHORT)y};
	if(DosConsole::StdFlag)//ok
	{
	SetConsoleCursorPosition( DosConsole::hOut, pos );
	return TRUE;
	}
	else
		return FALSE;
		
}
//************************************************

CONSOLE_SCREEN_BUFFER_INFO DosConsole::GetBufferInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer = {0};
	BOOL ScreenFlag;

	if(DosConsole::StdFlag)//ok
	{
		ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::hOut,&ScreenBuffer);
		return(CONSOLE_SCREEN_BUFFER_INFO) ScreenBuffer;
	}
	else 
		return (CONSOLE_SCREEN_BUFFER_INFO) ScreenBuffer;
}

//***************************************************

int DosConsole::MakeScreenBuffer()
{
	if(DosConsole::ActiveBuffers >= MAX_CONSOLEBUFFERS)
		return 0;
	DosConsole::NewScreenBuffer[DosConsole::ActiveBuffers] = CreateConsoleScreenBuffer(
	GENERIC_READ | GENERIC_WRITE ,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	DosConsole::ActiveBuffers++;
	return DosConsole::ActiveBuffers - 1;
}

//****************************************************
BOOL DosConsole::SetActiveBuffer(int Buffer)
{
	BOOL Active;
	if(Buffer > MAX_CONSOLEBUFFERS)
		return FALSE;
	Active =  SetConsoleActiveScreenBuffer(DosConsole::NewScreenBuffer[Buffer]);
	if(Active)
		return Active;
	else
		return Active;
}
//******************************************************

WORD DosConsole::SetBackAndTextActive(int TextColor ,int BackGroundColor,int Buffer)
{
	BOOL ScreenFlag,SetTextFlag;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	BackGroundColor = BackGroundColor << 4;
	TextColor       = TextColor + BackGroundColor;
	if(Buffer > MAX_CONSOLEBUFFERS)
		return FALSE;
	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::NewScreenBuffer[Buffer],&ScreenBuffer);
	SetTextFlag	= SetConsoleTextAttribute(DosConsole::NewScreenBuffer[Buffer],TextColor);
		return (WORD) ScreenBuffer.wAttributes;
}

//************************************************

CONSOLE_SCREEN_BUFFER_INFO DosConsole::GetBufferInfoActive(int Buffer)
{
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer = {0};
	BOOL ScreenFlag;
	if(Buffer > MAX_CONSOLEBUFFERS)
		return (CONSOLE_SCREEN_BUFFER_INFO) ScreenBuffer;
		ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::NewScreenBuffer[Buffer],&ScreenBuffer);
		return(CONSOLE_SCREEN_BUFFER_INFO) ScreenBuffer;

}

//***************************************************

BOOLEAN DosConsole::SetPosActive(int x,int y,int Buffer)
{
	COORD pos = {(SHORT)x, (SHORT)y};
	if(Buffer > MAX_CONSOLEBUFFERS)
		return FALSE;
	SetConsoleCursorPosition(DosConsole::NewScreenBuffer[Buffer],pos);
	return TRUE;		
}
//****************************************************************
DWORD DosConsole::SetScreenBackGroundActive(int BackGroundColor,int Buffer)
{
	BOOL ScreenFlag,SetFillFlag;
	DWORD Written;
	DWORD MaxCells;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBuffer;
	COORD cordpos;
	BackGroundColor = BackGroundColor << 4;
	cordpos.X = 0;
	cordpos.Y = 0;
	if(Buffer > MAX_CONSOLEBUFFERS)
		return FALSE;

	ScreenFlag	= GetConsoleScreenBufferInfo(DosConsole::NewScreenBuffer[Buffer],&ScreenBuffer);	
	MaxCells	= ScreenBuffer.dwMaximumWindowSize.X * ScreenBuffer.dwMaximumWindowSize.Y;
	SetFillFlag = FillConsoleOutputAttribute(DosConsole::NewScreenBuffer[Buffer],BackGroundColor,MaxCells,cordpos,&Written);
		return DWORD(Written);

}
//********************************************
BOOLEAN DosConsole::ReadInputRecordStd(INPUT_RECORD  *InputRecord,BOOLEAN Flush,DWORD *Read)//get infop fron STD_HANDLE
{
	BOOL InputFlag,FlushFlag,NumberFlag;
	DWORD Written,Amount;
	if(DosConsole::StdInFlag)//ok
	{
		if(Flush)
			{
        FlushFlag = FlushConsoleInputBuffer(DosConsole::hIn);
		if(!FlushFlag)
			return FALSE;
			}
		else
		if(!DosConsole::StdInFlag)
			return FALSE;

		NumberFlag = GetNumberOfConsoleInputEvents(DosConsole::hIn,&Amount);
		*Read = Amount;//set to amount that are waiting to be read
        if(Amount == 0)
			return FALSE;

		InputFlag = ReadConsoleInput(DosConsole::hIn,InputRecord,1,&Written);
		DosConsole::AmountReadInput++;
	    if(!InputFlag)
		{
	    DosConsole::LastError = GetLastError();
			return FALSE;
		}
	
		return TRUE;
	}
		return FALSE;
}
//************************************************
BOOLEAN DosConsole::ReadConseleOutputAttribeStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read)
{
	BOOL ReadFlag;
	DWORD Error;

	if(!DosConsole::StdFlag)
		return FALSE;

	ReadFlag = ReadConsoleOutputAttribute(DosConsole::hOut,BufferAttribe,Amount,StartRead,Read);
	if(!ReadFlag)
	{
		Error = GetLastError();
		DosConsole::GetMessageErrorString(Error);
		return FALSE;
	}
	else
		return TRUE;
}
//**********************************************
BOOLEAN DosConsole::WriteConsoleOutputAttribsStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read)
{
	BOOL WriteFlag;

	if(!DosConsole::StdFlag)
		return FALSE;

	WriteFlag = WriteConsoleOutputAttribute(DosConsole::hOut,BufferAttribe,Amount,StartRead,Read);
	if(!WriteFlag)
		return FALSE;
	else 
		return TRUE;
}

//**********************************************
BOOLEAN DosConsole::GetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCursorStd)
{

	BOOLEAN CursorFlag;
	if(DosConsole::StdFlag)//ok
	{
		CursorFlag = GetConsoleCursorInfo(DosConsole::hOut,&CursorInfoStd);
		memcpy(ScreenCursorStd,&CursorInfoStd,sizeof(CONSOLE_CURSOR_INFO));
	return TRUE;
	}

	return FALSE;
}

//********************************************
BOOLEAN DosConsole::SetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCursorInfo)
{

	BOOLEAN CursorFlag;
	if(DosConsole::StdFlag)//ok
	{
		CursorFlag = SetConsoleCursorInfo(DosConsole::hOut,ScreenCursorInfo);
		
	return TRUE;
	}
	return FALSE;
}
//********************************************

BOOLEAN DosConsole::SetConsoleTitleStd(char *string)
{
	BOOL TitleFlag;
	if(DosConsole::StdFlag)//ok
	{
		TitleFlag = SetConsoleTitleA(string);
		if(!TitleFlag)
			return FALSE;
	return TRUE;
	}

	return FALSE;
}
//********************************************
BOOLEAN DosConsole::GetConsoleTitleStd(char *string)
{
	BOOL TitleFlag;
	if(DosConsole::StdFlag)//ok
	{
		TitleFlag = GetConsoleTitleA(&ConsoleTitleStd[0],256);
		if(!TitleFlag)
			return FALSE;
		memcpy(string,&ConsoleTitleStd[0],256);
	return TRUE;
	}

	return FALSE;
}
//********************************************
DWORD DosConsole::GetMessageErrorString(DWORD ErrorNumber)
{
	DWORD BytesReturned;
	VOID *ErrorString;
 
		BytesReturned = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,ErrorNumber,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&ErrorString,0,NULL); 
  
	MessageBoxA( NULL, (LPCSTR)ErrorString, "Error", MB_OK | MB_ICONINFORMATION );
	LocalFree( ErrorString );
	
	return BytesReturned;
}
//*****************************************************************
bool DosConsole::GetStartTime()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&StartingTime.QuadPart);
	return true;
}
//*****************************************************************
bool DosConsole::GetEndTime()
{
	LARGE_INTEGER  TempBaseCounter;
	LARGE_INTEGER  TempCounter;

	QueryPerformanceCounter((LARGE_INTEGER *)&EndingTime.QuadPart);
	TempBaseCounter.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	TempCounter.QuadPart = TempBaseCounter.QuadPart;
	ElapsedNanoseconds = (double)(TempCounter.QuadPart *= 1000000000) / Frequency.QuadPart;
	TempCounter.QuadPart = TempBaseCounter.QuadPart;
	ElapsedMicroseconds = (double)(TempCounter.QuadPart *= 1000000) / Frequency.QuadPart;
	TempCounter.QuadPart = TempBaseCounter.QuadPart;
	ElapsedMilliseconds = (double)(TempCounter.QuadPart *= 1000) / Frequency.QuadPart;
	TempCounter.QuadPart = TempBaseCounter.QuadPart;
	ElapsedSeconds = (double)(TempCounter.QuadPart) / Frequency.QuadPart;
	if ((TempCounter.QuadPart != 0) && (Frequency.QuadPart != 0))
		return false;

	return true;
}
//************* ShowTextCpu() ***********
