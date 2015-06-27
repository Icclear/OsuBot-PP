#include "Play.h"

Play::Play(Beatmap ^beatmap, OsuManagement ^osu)
{
	std::cout << "Playing" << std::endl;

	LoadedBeatmap = beatmap;
	Osu = osu;

	BPMs = LoadedBeatmap->getBPMs();
	HitObjects = LoadedBeatmap->getHitObjects();

	Button1 = gcnew KlickButton;
	Button2 = gcnew KlickButton;

	Config config;
	System::String ^Value = config.getValueByKey("Button1", "x");
	Button1->Keycode = VkKeyScanEx(Value->ToCharArray()[0], GetKeyboardLayout(0)) & 0xFF;

	Value = config.getValueByKey("Button2", "z");
	Button2->Keycode = VkKeyScanEx(Value->ToCharArray()[0], GetKeyboardLayout(0)) & 0xFF;

	Button1->PressButton = InitButton(Button1->Keycode, true);
	Button1->ReleaseButton = InitButton(Button1->Keycode, false);
	Button2->PressButton = InitButton(Button2->Keycode, true);
	Button2->ReleaseButton = InitButton(Button2->Keycode, false);
}

Play::~Play()
{
	delete Button1->PressButton;
	delete Button2->PressButton;
	delete Button1->ReleaseButton;
	delete Button2->ReleaseButton;
	delete NextClick;
}

INPUT *Play::InitButton(WORD VirtualKeyCode, bool Press)
{
	INPUT *Button = new INPUT;
	Button->type = INPUT_KEYBOARD;
	Button->ki.wScan = 0;
	Button->ki.dwExtraInfo = 0;
	Button->ki.wVk = VirtualKeyCode;
	if (Press)
		Button->ki.dwFlags = 0;
	else
		Button->ki.dwFlags = KEYEVENTF_KEYUP;
	return Button;
}

void Play::StartPlaying()
{
	bool Playing = true;

	int Time;
	FoundNextHit = false;
	bool FinishedSong = false;

	Config config;

	//const int preKlick = System::Convert::ToInt32(config.getValueByKey("preKlick", "10"));
	double dtfactor = 1;
	if (DT)
		dtfactor = 1.4;
	const int preKlick = dtfactor * (3 + (78 - (LoadedBeatmap->getMapOverallDifficulty(HR) * 6)));

	const int extraPressTime = System::Convert::ToInt32(config.getValueByKey("extraPressTime", "35"));

	double currentBPM = BPMs[0]->Duration;
	double lastBPM = currentBPM;

	HitObject ^nextHit;
	NextClick = gcnew Relax;

	int BPMIterator = 0;
	int HitObjectsIterator = 0;

	ResetButtons();


	OsuBot::PlayUI PlayUI;
	PlayUI.setSong(LoadedBeatmap->getName());
	PlayUI.Show();

	char Title[0x10];
	//Osu->getWindowTitle(Title);
	//if (CString(Title) == CString("osu!"))
	do
	{
		Osu->readPlaying(Playing);
		if(!Playing)
		{
			return;
			PlayUI.Close();
		}

		Osu->readTime(Time);
		Sleep(100);
	} while (Time < HitObjects[0]->Time - 200 || Time > HitObjects[0]->Time);	//Wait for start

	std::cout << "\n\nTitle: " << CString(Osu->getWindowTitle()) << std::endl << std::endl;
	//TODO: Remove

	while (Playing)
	{
		Osu->readTime(Time);
		Osu->readPlaying(Playing);

		while (BPMIterator < BPMs->Count - 1 && HitObjectsIterator < HitObjects->Count &&
		HitObjects[HitObjectsIterator]->Time > BPMs[BPMIterator + 1]->Time - preKlick && Time >= 0)	//BPM change
		{
			BPMIterator++;
			if (BPMs[BPMIterator]->Duration < 0)	//Inherited BPM
				currentBPM = -1 * BPMs[BPMIterator]->Duration / 100 * lastBPM;
			else
			{
				currentBPM = lastBPM = BPMs[BPMIterator]->Duration;
			}
		}


		if (!FoundNextHit)
		{
			while (!FoundNextHit)
			{
				delete nextHit;

				if (HitObjectsIterator >= HitObjects->Count)
				{
					FinishedSong = true;
					if (Button1->Pressed || Button2->Pressed)
						break;
					std::cout << "Finished Playing." << std::endl;
					ResetButtons();
					PlayUI.Close();
					return;
				}
				if (Time < HitObjects[HitObjectsIterator]->Time)
				{
					FoundNextHit = true;
					nextHit = gcnew HitObject;
					nextHit = HitObjects[HitObjectsIterator];
				}
				HitObjectsIterator++;
			}

			if (!FinishedSong)
			{
				NextClick->BeginKlick = nextHit->Time - preKlick;

				if((nextHit->Type & 1) > 0)	//HitCircle
				{
					NextClick->EndKlick = NextClick->BeginKlick + extraPressTime;
				}

				else if((nextHit->Type & 8) > 0)	//Spin
				{
					NextClick->EndKlick = nextHit->SpinEndTime;

					if (HitObjectsIterator < HitObjects->Count - 1 && NextClick->EndKlick >= HitObjects[HitObjectsIterator + 1]->Time)
						NextClick->EndKlick = HitObjects[HitObjectsIterator + 1]->Time - extraPressTime;
				}
				else if ((nextHit->Type & 2) > 0)	//Slider
				{
					NextClick->EndKlick = nextHit->Time
						+ currentBPM * nextHit->Repetition * nextHit->PixelLength
						/ LoadedBeatmap->getMapSliderMultiplier() / 100;

					if (HitObjectsIterator < HitObjects->Count - 1 && NextClick->EndKlick >= HitObjects[HitObjectsIterator + 1]->Time)
						NextClick->EndKlick = HitObjects[HitObjectsIterator + 1]->Time - extraPressTime;
				}
				else
					std::cout << "Unknown Object, no.: " << HitObjectsIterator - 1 << std::endl;	//Error
			}
		}
		if (!FinishedSong)
		{
			if (Time > NextClick->BeginKlick && FoundNextHit)
			{
				FoundNextHit = false;
				Klick();
			}
		}

		ReleaseButtons(Time);
		if(Careaboutpreformance)
			Sleep(1);
	}
	std::cout << "Stopped Playing." << std::endl;
	ResetButtons();
	PlayUI.Close();
}

void Play::Klick()
{
	if (LastButtonPressed == 2)
		BT1Klick();
	else
		BT2Klick();
}

void Play::ReleaseButtons(const int &time)
{
	static const int &Time = time;
	if (Button1->Pressed && Time > Button1->ReleaseAt)
	{
		SendInput(1, Button1->ReleaseButton, sizeof(INPUT));
		Button1->Pressed = false;
	}
	if (Button2->Pressed && Time > Button2->ReleaseAt)
	{
		SendInput(1, Button2->ReleaseButton, sizeof(INPUT));
		Button2->Pressed = false;
	}
}

void Play::ResetButtons()
{
	SendInput(1, Button1->ReleaseButton, sizeof(INPUT));
	Button1->Pressed = false;

	SendInput(1, Button2->ReleaseButton, sizeof(INPUT));
	Button2->Pressed = false;
}

void Play::BT1Klick()
{
	if (!Button1->Pressed)
	{
		SendInput(1, Button1->PressButton, sizeof(INPUT));
		Button1->Pressed = true;
		Button1->ReleaseAt = NextClick->EndKlick;
	}
	else if (!Button2->Pressed)
	{
		SendInput(1, Button2->PressButton, sizeof(INPUT));
		Button2->Pressed = true;
		Button2->ReleaseAt = NextClick->EndKlick;
	}
	else
	{	//forcerelease
		FoundNextHit = true;
		SendInput(1, Button1->ReleaseButton, sizeof(INPUT));
		Button1->Pressed = false;
		return;
	}
	LastButtonPressed = 1;
}

void Play::BT2Klick()
{
	if (!Button2->Pressed)
	{
		SendInput(1, Button2->PressButton, sizeof(INPUT));
		Button2->Pressed = true;
		Button2->ReleaseAt = NextClick->EndKlick;
	}
	else if (!Button1->Pressed)
	{
		SendInput(1, Button1->PressButton, sizeof(INPUT));
		Button1->Pressed = true;
		Button1->ReleaseAt = NextClick->EndKlick;
	}
	else
	{	//forcerelease
		FoundNextHit = true;
		SendInput(1, Button2->ReleaseButton, sizeof(INPUT));
		Button2->Pressed = false;
		return;
	}
	LastButtonPressed = 2;
}