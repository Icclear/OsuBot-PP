#include <iostream>
#include <fstream> 
#include <atlstr.h>		//CString

#ifndef BEATMAP_H
#define BEATMAP_H

ref struct BPM
{
	int Time;
	double Duration;
};

ref struct HitObject
{
	int x, y;
	int Time;
	int Type;
	System::String ^SliderType;
	int Repetition;
	double PixelLength;
	int SpinEndTime;
};

#pragma once
ref class Beatmap
{
public:
	Beatmap();

	void LoadBeatmap(System::String ^BeatmapPath);

	int getError();
	int getVersion();
	int getMode();
	double getMapCircleSize();
	double getMapOverallDifficulty();
	double getMapApproachRate();
	double getMapSliderMultiplier();
	System::Collections::Generic::List<BPM^> ^getBPMs();
	System::Collections::Generic::List<HitObject^> ^getHitObjects();

private:
	bool BeatmapLoaded;
	unsigned int Error;
	unsigned int MapVersion;
	unsigned int MapMode;
	double MapCircleSize;
	double MapOverallDifficulty;
	double MapApproachRate;
	double MapSliderMultiplier;

	System::String ^Path;
	array<System::String^> ^BeatmapFile;
	System::Collections::Generic::List<BPM^> ^BPMs;
	System::Collections::Generic::List<HitObject^> ^HitObjects;


	void readBeatmap();
	void readVersion();
	void readMode();
	void readDifficulty();
	void readBPMs();
	void readHitObjects();
};

#endif