#include <windows.h>
#include "AOBScan.h"
#include <iostream>
#include <atlstr.h>		//CString

#ifndef OSUMANAGEMENT_H
#define OSUMANAGEMENT_H

#pragma once
ref class OsuManagement
{
private:
	HWND Window;
	DWORD ProcessID;
	HANDLE ProcessHandle;
	LPVOID TimeAdress;
	LPVOID PlayingAdress;
	System::String ^Filepath;
	int Error;
public:
	OsuManagement();
	void readTime(int &Time);
	void readPlaying(bool &Playing);
	int getError();
	System::String ^getFilepath();
	System::String ^getWindowTitle();
};

#endif