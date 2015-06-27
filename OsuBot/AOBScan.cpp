#include "AOBScan.h"

DWORD AOBScan::FindPattern(HANDLE ProcessHandle, BYTE Signature[], unsigned const int ByteCount)
{
	const static unsigned int Mult = 4096;
	const static unsigned int StartAdress = 0x00000000;
	const static unsigned int EndAdress = 0x7FFFFFFF;
	static bool Hit;

	BYTE ByteInMemory[Mult];

	for (unsigned int i = StartAdress; i < EndAdress; i += Mult - ByteCount)
	{
		ReadProcessMemory(ProcessHandle, (LPCVOID)i, &ByteInMemory, Mult, NULL);
		for (unsigned int a = 0; a < Mult; a++)
		{
			Hit = true;

			for (unsigned int j = 0; j < ByteCount && Hit; j++)
			{
				if (ByteInMemory[a + j] != Signature[j])
					Hit = false;
			}
			if (Hit)
			{
				return i + a;
			}
		}
	}
	return 0;
}
