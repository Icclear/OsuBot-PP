#include "Config.h"

using System::IO::File;

Config::Config()
{
	if (!File::Exists(System::IO::Directory::GetCurrentDirectory() + Dir + File))
	{
		CreateIni();
	}
	if (Error != 0)
		return;
	SettingsFile = File::ReadAllLines(System::IO::Directory::GetCurrentDirectory() + Dir + File);	

	if (SettingsFile->Length == 0)
		std::cout << "Error reading settings." << std::endl;
}

System::String ^Config::getValueByKey(System::String ^Key)
{
	if (Error != 0)
		return "";

	for (int i = 0; i < SettingsFile->Length; i++)
	{
		if (SettingsFile[i]->Contains(Key))
		{
			return SettingsFile[i]->Substring(SettingsFile[i]->IndexOf("=") + 1)->Trim();
		}
	}
	return "";
}

void Config::CreateIni()
{
	if (!System::IO::Directory::Exists(System::IO::Directory::GetCurrentDirectory() + Dir))
	{
		System::IO::Directory::CreateDirectory(System::IO::Directory::GetCurrentDirectory() + Dir);
	}

	System::IO::FileStream ^file = File::Create(System::IO::Directory::GetCurrentDirectory() + Dir + File);
	file->Close();

	FILE *IniFile = fopen(CString(System::IO::Directory::GetCurrentDirectory() + Dir + File), "w");
	if (IniFile == nullptr)
	{
		std::cout << "Error creating File." << std::endl;
		Error = 1;
		return;
	}
	fprintf_s(IniFile, "Button1=x\n");
	fprintf_s(IniFile, "Button2=z\n");
	fclose(IniFile);
}