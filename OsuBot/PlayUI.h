#ifndef PLAYUI_H
#define PLAYUI_H

#pragma once

namespace OsuBot {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für PlayUI
	/// </summary>
	public ref class PlayUI : public System::Windows::Forms::Form
	{
	public:
		PlayUI(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}
		void setSong(System::String ^Song)
		{
			this->Text = Song;
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~PlayUI()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// PlayUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::EnableAllowFocusChange;
			this->ClientSize = System::Drawing::Size(372, 0);
			this->ControlBox = false;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"PlayUI";
			this->Opacity = 0.3;
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"PlayUI";
			this->TopMost = true;
			this->MouseEnter += gcnew System::EventHandler(this, &PlayUI::PlayUI_MouseEnter);
			this->MouseLeave += gcnew System::EventHandler(this, &PlayUI::PlayUI_MouseLeave);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void PlayUI_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		Cursor->Hide();
	}
	private: System::Void PlayUI_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		Cursor->Show();
	}
	};
}

#endif