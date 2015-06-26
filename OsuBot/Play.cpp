#include "Play.h"



Play::Play(Beatmap ^beatmap, OsuManagement ^osu)
{
	std::cout << "Playing" << std::endl;

	LoadedBeatmap = beatmap;
	Osu = osu;

	BPMs = LoadedBeatmap->getBPMs();
	HitObjects = LoadedBeatmap->getHitObjects();

	Button1 = gcnew Button;
	Button2 = gcnew Button;

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

INPUT *Play::InitButton(DWORD VirtualKeyCode, bool Press)
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
	bool FoundNextHit = false;
	bool FinishedSong = false;

	const int preKlick = 10;
	const int extraPressTime = 25;

	double currentBPM = BPMs[0]->Duration;
	double lastBPM = currentBPM;

	HitObject ^nextHit;
	NextClick = gcnew Relax;

	int BPMIterator = 0;
	int HitObjectsIterator = 0;

	while (Playing)
	{
		Osu->readTime(Time);
		if (BPMIterator < BPMs->Count - 1 && Time > BPMs[BPMIterator + 1]->Time)	//BPM change
		{
			BPMIterator++;
			if (BPMs[BPMIterator]->Duration < 0)	//Inherited BPM
				currentBPM = -1 * BPMs[BPMIterator]->Duration / 100 * lastBPM;
			else
			{
				currentBPM = lastBPM = BPMs[BPMIterator]->Duration;
			}
			//TODO: Inherited points bugged
		}

		if (!FoundNextHit)
		{
			while (!FoundNextHit)
			{//TODO: Some hits are left out (Long stream practice)
				delete nextHit;
				if (HitObjectsIterator >= HitObjects->Count)
				{
					FinishedSong = true;
					if (Button1->Pressed || Button2->Pressed)
						break;
					std::cout << "Finished Playing." << std::endl;
					return;
				}
				if (Time < HitObjects[HitObjectsIterator]->Time)
				{
					FoundNextHit = true;
					KlickedHit = false;
					nextHit = gcnew HitObject;
					nextHit = HitObjects[HitObjectsIterator];
				}
				HitObjectsIterator++;
			} 

			if (!FinishedSong)
			{
				NextClick->BeginKlick = nextHit->Time - preKlick;

				if (nextHit->Type == 1 || nextHit->Type == 5 || nextHit->Type == 16	//HitCircle
					|| nextHit->Type == 37 || nextHit->Type == 21)
				{
					NextClick->EndKlick = NextClick->BeginKlick + extraPressTime;
				}
				else if (nextHit->Type == 12 || nextHit->Type == 8)	//Spin
				{
					NextClick->EndKlick = nextHit->SpinEndTime + extraPressTime;

					if (BPMIterator < BPMs->Count - 1 && NextClick->EndKlick >= BPMs[BPMIterator + 1]->Time)
						NextClick->EndKlick = BPMs[BPMIterator + 1]->Time - extraPressTime * 2;
					//TODO: Spin at the end
				}
				else		//Slider
				{
					NextClick->EndKlick = NextClick->BeginKlick
						+ currentBPM * nextHit->Repetition * nextHit->PixelLength
						/ LoadedBeatmap->getMapSliderMultiplier() / 100 + extraPressTime;

					if (BPMIterator < BPMs->Count - 1 && NextClick->EndKlick >= BPMs[BPMIterator + 1]->Time)
						NextClick->EndKlick = BPMs[BPMIterator + 1]->Time - extraPressTime * 2;
				}
			}
		}
		if (!FinishedSong)
		{
			if (Time > NextClick->BeginKlick && Time < NextClick->EndKlick && !KlickedHit)
			{
				KlickedHit = true;
				Klick();
			}
			else if(KlickedHit)
			{
				KlickedHit = false;
				FoundNextHit = false;
			}
		}
		ReleaseButtons(Time);
		//TODO: Fix random stopping
	} 
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
		KlickedHit = false;
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
		KlickedHit = false;
		SendInput(1, Button2->ReleaseButton, sizeof(INPUT));
		Button2->Pressed = false;
		return;
	}
	LastButtonPressed = 2;
}