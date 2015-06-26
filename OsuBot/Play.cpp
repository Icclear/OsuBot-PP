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

	Button1->PressButton = InitButton(VK_OEM_MINUS, true);
	Button1->ReleaseButton = InitButton(VK_OEM_MINUS, false);
	Button2->PressButton = InitButton(VK_OEM_PERIOD, true);
	Button2->ReleaseButton = InitButton(VK_OEM_PERIOD, false);

	StartPlaying();
}

/*		
		ipm.ki.dwFlags = 0; //Keydown
		while (true)
		{
			SendInput(1, &ipm, sizeof(INPUT));
			Sleep(5000);
		}
		*/

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

	const int preKlick = 10;
	const int extraPressTime = 25;

	double currentBPM = BPMs[0]->Duration;
	double lastBPM = currentBPM;

	HitObject ^nextHit;
	NextClick = gcnew Relax;

	int BPMIterator = 0;
	int HitObjectsIterator = 0;

	ResetButtons();

	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x53);
	MSG HotKey;

	char Title[0x10];
	do
	{
		Osu->getWindowTitle(Title);
		if (CString(Title) == CString("osu!"))	//Not playing at all
			return;

		Osu->readTime(Time);
		Sleep(5);
	} while (Time < HitObjects[0]->Time - 200 || Time > HitObjects[0]->Time);	//Wait for start

	while (Playing)
	{
		if (Time % 200 == 0)	//Hotkey for stop pressed?
		{
			GetMessage(&HotKey, NULL, NULL, NULL);
			if (HotKey.message == WM_HOTKEY)
				if (HotKey.wParam == 1)
				Playing = false;
		}

		Osu->readTime(Time);
		while (BPMIterator < BPMs->Count - 1 && Time > BPMs[BPMIterator + 1]->Time && Time >= 0)	//BPM change
		{
			BPMIterator++;
			if (BPMs[BPMIterator]->Duration < 0)	//Inherited BPM
				currentBPM = -1 * BPMs[BPMIterator]->Duration / 100 * lastBPM;
			else
			{
				currentBPM = lastBPM = BPMs[BPMIterator]->Duration;
			}
			//TODO: Inherited points bugged?
			continue;
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
					UnregisterHotKey(NULL, 1);
					ResetButtons();
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

					if (BPMIterator < BPMs->Count - 1 && NextClick->EndKlick >= BPMs[BPMIterator + 1]->Time)
						NextClick->EndKlick = BPMs[BPMIterator + 1]->Time - extraPressTime;
					//TODO: Some spins don't work
				}
				else if ((nextHit->Type & 2) > 0)	//Slider
				{
					NextClick->EndKlick = NextClick->BeginKlick
						+ currentBPM * nextHit->Repetition * nextHit->PixelLength
						/ LoadedBeatmap->getMapSliderMultiplier() / 100;

					if (BPMIterator < BPMs->Count - 1 && NextClick->EndKlick >= BPMs[BPMIterator + 1]->Time)
						NextClick->EndKlick = BPMs[BPMIterator + 1]->Time - extraPressTime;
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
	}
	std::cout << "Stopped Playing." << std::endl;
	UnregisterHotKey(NULL, 1);
	ResetButtons();
}

void Play::Klick()
{
	if (LastButtonPressed == 2)
		BT1Klick();
	else
		BT2Klick();
}

void Play::ReleaseButtons(const int &Time)
{
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