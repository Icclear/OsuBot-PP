#include <windows.h>

#ifndef AOBSCAN_H
#define AOBSCAN_H

#pragma once
ref class AOBScan
{
public:
	static DWORD FindPattern(HANDLE ProcessHandle, BYTE Signature[], unsigned const int ByteCount);
};

#endif