#include <Windows.h>
#include "Beatmap.h"
#include "OsuManagement.h"

#ifndef PLAY_H
#define PLAY_H

#pragma once

ref struct Relax
{
	int BeginKlick;
	int EndKlick;
};

ref struct Button
{
	bool Pressed;
	INPUT *PressButton;
	INPUT *ReleaseButton;
	int ReleaseAt;
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

	Button ^Button1;
	Button ^Button2;

	int LastButtonPressed = 1;
	bool KlickedHit;

	System::Collections::Generic::List<BPM^> ^BPMs;
	System::Collections::Generic::List<HitObject^> ^HitObjects;

	INPUT *InitButton(DWORD ButtonNR, bool Press);

	void Klick();
	void ReleaseButtons(const int &Time);
	void BT1Klick();
	void BT2Klick();
};

#endif