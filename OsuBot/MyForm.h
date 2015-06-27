#include<iostream>
#include <atlstr.h>		//CString
#include "OsuManagement.h"
#include "Beatmap.h"
#include "Play.h"
#include "Config.h"

#ifndef MYFORM_H
#define MYFORM_H

#pragma once

namespace OsuBot {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void);

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}



	private: OsuManagement ^Osu;
	private: Beatmap ^LoadedBeatmap;
	private: bool BeatmapLoaded = false;
	private: array<System::String^>^ FolderList;
	private: array<System::String^>^ DiffList;
	private: System::Windows::Forms::ListBox^  SongListBox;
	private: System::Windows::Forms::Button^  LoadBeatmapButton;



	private: System::Windows::Forms::ListBox^  DiffListBox;
	private: System::Windows::Forms::Timer^  Playcheck;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  LoadedBeatmapLabel;


	private: System::Windows::Forms::TextBox^  Songsearch;



	private: void LoadSongList();
	private: void SearchSongsearch();
	private: void LoadDiffList();
	private: void LoadSelectedBeatmap();
	private: void checkForPlaying();
	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->SongListBox = (gcnew System::Windows::Forms::ListBox());
			this->LoadBeatmapButton = (gcnew System::Windows::Forms::Button());
			this->Songsearch = (gcnew System::Windows::Forms::TextBox());
			this->DiffListBox = (gcnew System::Windows::Forms::ListBox());
			this->Playcheck = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->LoadedBeatmapLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// SongListBox
			// 
			this->SongListBox->FormattingEnabled = true;
			resources->ApplyResources(this->SongListBox, L"SongListBox");
			this->SongListBox->Name = L"SongListBox";
			this->SongListBox->Click += gcnew System::EventHandler(this, &MyForm::SongListBox_Click);
			this->SongListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::listBox1_SelectedIndexChanged);
			// 
			// LoadBeatmapButton
			// 
			resources->ApplyResources(this->LoadBeatmapButton, L"LoadBeatmapButton");
			this->LoadBeatmapButton->Name = L"LoadBeatmapButton";
			this->LoadBeatmapButton->UseVisualStyleBackColor = true;
			this->LoadBeatmapButton->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// Songsearch
			// 
			resources->ApplyResources(this->Songsearch, L"Songsearch");
			this->Songsearch->Name = L"Songsearch";
			this->Songsearch->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::Songsearch_KeyDown);
			// 
			// DiffListBox
			// 
			this->DiffListBox->FormattingEnabled = true;
			resources->ApplyResources(this->DiffListBox, L"DiffListBox");
			this->DiffListBox->Name = L"DiffListBox";
			// 
			// Playcheck
			// 
			this->Playcheck->Interval = 10;
			this->Playcheck->Tick += gcnew System::EventHandler(this, &MyForm::Playcheck_Tick);
			// 
			// label1
			// 
			resources->ApplyResources(this->label1, L"label1");
			this->label1->Name = L"label1";
			// 
			// label2
			// 
			resources->ApplyResources(this->label2, L"label2");
			this->label2->Name = L"label2";
			// 
			// LoadedBeatmapLabel
			// 
			resources->ApplyResources(this->LoadedBeatmapLabel, L"LoadedBeatmapLabel");
			this->LoadedBeatmapLabel->Name = L"LoadedBeatmapLabel";
			// 
			// MyForm
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->LoadedBeatmapLabel);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->DiffListBox);
			this->Controls->Add(this->Songsearch);
			this->Controls->Add(this->LoadBeatmapButton);
			this->Controls->Add(this->SongListBox);
			this->Name = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		LoadSelectedBeatmap();
	}
private: System::Void Songsearch_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	if (e->KeyCode == Keys::Enter)
		SearchSongsearch();
}
private: System::Void SongListBox_Click(System::Object^  sender, System::EventArgs^  e) {
	LoadDiffList();
}
private: System::Void Playcheck_Tick(System::Object^  sender, System::EventArgs^  e) {
	checkForPlaying();
}
};
}

#endif