
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
}CpuData;
#pragma pack (pop)