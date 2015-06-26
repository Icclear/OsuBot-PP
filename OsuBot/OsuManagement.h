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
	System::String ^Filepath;
	int Error;
public:
	OsuManagement();
	void readTime(int &Time);
	int getError();
	System::String ^getFilepath();
	void getWindowTitle(char *Path);
};

#endif