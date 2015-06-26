#include <iostream>

#ifndef CONFIG_H
#define CONFIG_H

#pragma once
ref class Config
{
public:
	Config();
	System::String ^getValueByKey(System::String ^Key);

private:
	array<System::String^> ^SettingsFile;

	int Error = 0;

	static System::String ^Dir = "/Data";
	static System::String ^File = "/Settings.ini";
};

#endif