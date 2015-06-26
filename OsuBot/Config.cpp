#include "Config.h"



Config::Config()
{
	if (!System::IO::File::Exists(System::IO::Directory::GetCurrentDirectory() + Dir + File))
	{
		Error = 1;
		return;
	}
	SettingsFile = System::IO::File::ReadAllLines(System::IO::Directory::GetCurrentDirectory() + Dir + File);	

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