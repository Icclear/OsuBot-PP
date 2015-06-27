#include <iostream>
#include <atlstr.h>		//CString
#include <fstream>

#ifndef CONFIG_H
#define CONFIG_H

#pragma once
ref class Config
{
public:
	Config();
	System::String ^getValueByKey(System::String ^Key, System::String ^Default);

private:
	array<System::String^> ^SettingsFile;

	int Error = 0;

	void CreateIni();

	static System::String ^Dir = "/Data";
	static System::String ^File = "/Settings.ini";
};

#endif