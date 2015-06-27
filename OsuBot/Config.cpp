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

System::String ^Config::getValueByKey(System::String ^Key, System::String ^Default)
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

	std::ofstream IniFile(CString(System::IO::Directory::GetCurrentDirectory() + Dir + File), std::ios_base::app);
	IniFile << CString(Key + "=" + Default) << std::endl;
	IniFile.close();

	return Default;
}

void Config::CreateIni()
{
	if (!System::IO::Directory::Exists(System::IO::Directory::GetCurrentDirectory() + Dir))
	{
		System::IO::Directory::CreateDirectory(System::IO::Directory::GetCurrentDirectory() + Dir);
	}

	System::IO::FileStream ^file = File::Create(System::IO::Directory::GetCurrentDirectory() + Dir + File);
	file->Close();


	if (!System::IO::Directory::Exists(System::IO::Directory::GetCurrentDirectory() + Dir))
		Error = 1;
}