#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThread]

int main()
//int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	OsuBot::MyForm form;
	Application::Run(%form);
}

OsuBot::MyForm::MyForm(void)
{
	InitializeComponent();

	//
	//TODO: Konstruktorcode hier hinzufügen.
	//
	Osu = gcnew OsuManagement();
	LoadedBeatmap = gcnew Beatmap();
	LoadSongList();
}

void OsuBot::MyForm::LoadSongList()
{
	System::String^ Songsfolder = Osu->getFilepath();	//Osu Folder
	FolderList = System::IO::Directory::GetDirectories(Songsfolder);	//Songlist
	for (int i = 0; i < FolderList->Length; i++)
	{
		//remove the path but the actual directoryname
		FolderList[i] = FolderList[i]->Substring(Songsfolder->Length, FolderList[i]->Length - Songsfolder->Length);
	}
}

void OsuBot::MyForm::SearchSongsearch()
{
	SongListBox->Items->Clear();
	DiffListBox->Items->Clear();

	for (int i = 0; i < FolderList->Length; i++)
	{
		if (FolderList[i]->ToLower()->Contains(Songsearch->Text->ToLower()))
		{
			SongListBox->Items->Add(FolderList[i]);
		}
	}
}

void OsuBot::MyForm::LoadDiffList()
{
	DiffListBox->Items->Clear();

	System::String^ Songfolder = Osu->getFilepath() + SongListBox->SelectedItem;
	DiffList = System::IO::Directory::GetFiles(Songfolder, "*.osu");
	for (int i = 0; i < DiffList->Length; i++)
	{
		System::String ^Filename = DiffList[i]->Substring(DiffList[i]->IndexOf("["),
			DiffList[i]->LastIndexOf("]") - DiffList[i]->IndexOf("[") + 1);
		DiffListBox->Items->Add(Filename);
	}
}

void OsuBot::MyForm::LoadSelectedBeatmap()
{
	if (System::String::IsNullOrWhiteSpace(DiffListBox->SelectedItem + ""))
		return;

	System::String ^SelectedDiff;
	for (int i = 0; i < DiffList->Length; i++)
	{
		if (DiffList[i]->Contains("" + DiffListBox->SelectedItem))
			SelectedDiff = DiffList[i];
	}

	LoadedBeatmap->LoadBeatmap(SelectedDiff);

	if (LoadedBeatmap->getError() == 0 && (LoadedBeatmap->getMode() == 0 
			|| LoadedBeatmap->getMode() == 4 && LoadedBeatmap->getVersion() <= 3))	//TODO: Find out the actual version mode was implemented.
		BeatmapLoaded = true;
	else
		BeatmapLoaded = false;

	if (BeatmapLoaded)
	{
		Playcheck->Enabled = true;

		LoadedBeatmapLabel->Text = "Loaded Beatmap: \n" + LoadedBeatmap->getName();
	}
	else
	{
		Playcheck->Enabled = false;
		std::cout << "MapVersion: " << LoadedBeatmap->getVersion() << " & Mode: " << LoadedBeatmap->getMode() << std::endl;
		//TODO: Remove this line later.

		LoadedBeatmapLabel->Text = "Loaded Beatmap: ";
	}
}

void OsuBot::MyForm::checkForPlaying()
{
	char Title[0x10];
	Osu->getWindowTitle(Title);
	if (CString(Title) != CString("osu!"))
	{
		Play ^Playing = gcnew Play(LoadedBeatmap, Osu);
	}

		//TODO: PLAY
}