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

ref struct WindowRatio
{
	static const int MaxX = 512;
	static const int MaxY = 384;
	static double Scale;
	static int MarginTop;
	static int MarginLeft;
	double X;
	double Y;
};

ref class Play
{
public:
	Play(Beatmap ^beatmap, OsuManagement ^osu);
	~Play();
	void StartPlaying();
	void activateHR() { HR = true; }
	void activateDT() { DT = true; }
	void FuckPerformance() { Careaboutpreformance = false; }

private:
	Beatmap ^LoadedBeatmap;
	OsuManagement ^Osu;
	Relax ^NextClick;

	KlickButton ^Button1;
	KlickButton ^Button2;

	RECT *OsuRec;
	WindowRatio ^WinRatio;

	int LastButtonPressed = 1;
	bool FoundNextHit;
	bool HR = false;
	bool DT = false;
	bool Careaboutpreformance = true;

	System::Collections::Generic::List<BPM^> ^BPMs;
	System::Collections::Generic::List<HitObject^> ^HitObjects;

	INPUT *InitButton(WORD ButtonNR, bool Press);

	void Klick();
	void MoveMouse(int x, int y);
	void ReleaseButtons(const int &time);
	void ResetButtons();
	void BT1Klick();
	void BT2Klick();
};

#endif