#include <Windows.h>
#include "Beatmap.h"
#include "OsuManagement.h"
#include "PlayUI.h"
#include "Config.h"

#ifndef PLAY_H
#define PLAY_H

#pragma once

ref struct Relax
{
	int BeginKlick;
	int EndKlick;
};

ref struct KlickButton
{
	bool Pressed;
	INPUT *PressButton;
	INPUT *ReleaseButton;
	int ReleaseAt;
	int Keycode;
};

ref class Play
{
public:
	Play(Beatmap ^beatmap, OsuManagement ^osu);
	~Play();
	void StartPlaying();

private:
	Beatmap ^LoadedBeatmap;
	OsuManagement ^Osu;
	Relax ^NextClick;

	KlickButton ^Button1;
	KlickButton ^Button2;

	int LastButtonPressed = 1;
	bool FoundNextHit;

	System::Collections::Generic::List<BPM^> ^BPMs;
	System::Collections::Generic::List<HitObject^> ^HitObjects;

	INPUT *InitButton(WORD ButtonNR, bool Press);

	void Klick();
	void ReleaseButtons(const int &time);
	void ResetButtons();
	void BT1Klick();
	void BT2Klick();
};

#endif