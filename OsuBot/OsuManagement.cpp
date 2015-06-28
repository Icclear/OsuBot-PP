#include "OsuManagement.h"

OsuManagement::OsuManagement()
{
	Error = 0;
	std::cout << "Searching Window." << std::endl;

	Window = FindWindow(0, (LPCSTR)"osu!");
	if (Window == 0 || Window == nullptr)
	{
		Error = 1;
		std::cout << "Couldn't find OsuWindow." << std::endl;
		return;
	}

	std::cout << "Searching ProcessID." << std::endl;

	DWORD pID;
	GetWindowThreadProcessId(Window, &pID);
	ProcessID = pID;

	if (ProcessID == 0 || ProcessID == NULL)
	{
		Error = 2;
		std::cout << "Couldn't find ProcessID." << std::endl;
		return;
	}

	std::cout << "Opening Process." << std::endl;

	ProcessHandle = OpenProcess(PROCESS_VM_READ, false, ProcessID);
	if (ProcessHandle == 0 || ProcessHandle == nullptr)
	{
		Error = 3;
		std::cout << "Couldn't open Process." << std::endl;
		return;
	}

	std::cout << "Searching Signature in Process." << std::endl;

	BYTE Pattern[] = { 0xB4, 0x17, 0x00, 0x00, 0x14, 0x13, 0x00, 0x00,
		0xB8, 0x17, 0x00 ,0x00, 0x14, 0x13, 0x00, 0x00 };

	int PatternLength = sizeof(Pattern) / sizeof(BYTE);

	DWORD ScanAdress = AOBScan::FindPattern(ProcessHandle, Pattern, PatternLength);
	if (ScanAdress == 0)
	{
		Error = 4;
		std::cout << "Couldn't find pattern." << std::endl;
		return;
	}

	std::cout << "Initializing Filepath and Timeadress." << std::endl;

	Filepath = System::Diagnostics::Process::GetProcessById(ProcessID)->Modules[0]->FileName;

	System::String ^OsuExe("osu!.exe");
	Filepath = Filepath->Substring(0, Filepath->Length - OsuExe->Length);	//Remove osu!.exe
	Filepath = System::String::Concat(Filepath, "Songs\\");					//add Songs\

	TimeAdress = (LPVOID)(ScanAdress + 0xA20);				
	PlayingAdress = (LPVOID)(ScanAdress + 0xA20 + 0xA34);	

	std::cout << "Success. Everything worked so far." << std::endl;
}

void OsuManagement::readTime(int &Time)
{
	if(Error == 0)
		ReadProcessMemory(ProcessHandle, TimeAdress, &Time, sizeof(Time), NULL);
}

void OsuManagement::readPlaying(bool &Playing)
{
	if (Error == 0)
		ReadProcessMemory(ProcessHandle, PlayingAdress, &Playing, sizeof(Playing), NULL);
}

int OsuManagement::getError()
{
	return Error;
}

System::String ^OsuManagement::getFilepath()
{
	if(Error == 0)
		return Filepath;
	return "";
}

System::String ^OsuManagement::getWindowTitle()
{

	if (Error == 0)
	{
		const static int Length = 256;
		char Title[Length];
		GetWindowText(Window, (LPSTR)Title, Length);
		return gcnew System::String(Title);
	}
	return "";
}

HWND OsuManagement::getWindow()
{
	return Window;
}