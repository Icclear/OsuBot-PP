#include "Beatmap.h"


Beatmap::Beatmap()
{

}

void Beatmap::LoadBeatmap(System::String ^BeatmapPath)
{
	Error = 0;
	BeatmapLoaded = false;
	std::cout << "Loading Beatmap..." << std::endl;
	Path = BeatmapPath;
	readBeatmap();
	readVersion();
	readDifficulty();
	readBPMs();
	readHitObjects();
	BeatmapLoaded = true;
}

void Beatmap::readBeatmap()
{
	BeatmapFile = System::IO::File::ReadAllLines(Path);

	if (BeatmapFile->Length == 0)
	{
		Error = 1;
		std::cout << "Something went wrong loading Beatmap: " << CString(Path) << std::endl;
		return;
	}
	std::cout << "First Line: " << CString(BeatmapFile[0]) << std::endl;

	if (!BeatmapFile[0]->Contains("osu file format v"))
	{
		Error = 2;
		std::cout << "Something went wrong loading Beatmap: " << CString(Path) << std::endl;
		return;
	}
	std::cout << "Beatmap successfully loaded: " << CString(Path) << std::endl;
}

void Beatmap::readVersion()
{
	if (Error != 0)
		return;
	
	System::String ^OsuFileFormat("osu file format v");
	
	MapVersion = System::Convert::ToInt32(BeatmapFile[0]->Substring(OsuFileFormat->Length,
					BeatmapFile[0]->Length - OsuFileFormat->Length)->Trim());
	std::cout << "Mapversion: " << MapVersion << std::endl;
}

void Beatmap::readDifficulty()
{
	System::String ^CircleSize("CircleSize:");
	System::String ^OverallDifficulty("OverallDifficulty:");
	System::String ^ApproachRate("ApproachRate:");
	System::String ^SliderMultiplier("SliderMultiplier:");

	int FoundCount = 0;

	for (int i = 1; i < BeatmapFile->Length && FoundCount < 4; i++)
	{
		if (BeatmapFile[i]->Contains(CircleSize))
		{
			BeatmapFile[i] = BeatmapFile[i]->Replace(".", ",");
			MapCircleSize = System::Convert::ToDouble(BeatmapFile[i]->Substring(CircleSize->Length,
				BeatmapFile[i]->Length - CircleSize->Length)->Trim());
			FoundCount++;
		}
		if (BeatmapFile[i]->Contains(OverallDifficulty))
		{
			BeatmapFile[i] = BeatmapFile[i]->Replace(".", ",");
			MapOverallDifficulty = System::Convert::ToDouble(BeatmapFile[i]->Substring(OverallDifficulty->Length,
				BeatmapFile[i]->Length - OverallDifficulty->Length)->Trim());
			FoundCount++;
		}
		if (BeatmapFile[i]->Contains(ApproachRate))
		{
			BeatmapFile[i] = BeatmapFile[i]->Replace(".", ",");
			MapApproachRate = System::Convert::ToDouble(BeatmapFile[i]->Substring(ApproachRate->Length,
				BeatmapFile[i]->Length - ApproachRate->Length)->Trim());
			FoundCount++;
		}
		if (BeatmapFile[i]->Contains(SliderMultiplier))
		{
			BeatmapFile[i] = BeatmapFile[i]->Replace(".", ",");
			MapSliderMultiplier = System::Convert::ToDouble(BeatmapFile[i]->Substring(SliderMultiplier->Length,
				BeatmapFile[i]->Length - SliderMultiplier->Length)->Trim());
			FoundCount++;
		}
	}
}

void Beatmap::readBPMs()
{
	BPMs = gcnew System::Collections::Generic::List<BPM^>;
	int i = 0;
	while (!BeatmapFile[i]->Contains("[TimingPoints]"))
		i++;
	i++;
	array<System::String^> ^Line;
	while (BeatmapFile[i]->Contains(","))
	{
		Line = BeatmapFile[i]->Split(',');
		BPM ^bpm = gcnew BPM;
		bpm->Time = System::Convert::ToInt32(Line[0]);
		bpm->Duration = System::Convert::ToDouble(Line[1]->Replace(".", ","));
		BPMs->Add(bpm);
		i++;
	}
}

void Beatmap::readHitObjects()
{
	HitObjects = gcnew System::Collections::Generic::List<HitObject^>;
	int i = 0;
	while (!BeatmapFile[i]->Contains("[HitObjects]"))
		i++;
	i++;
	array<System::String^> ^Line;
	while (i < BeatmapFile->Length && BeatmapFile[i]->Contains(","))
	{
		Line = BeatmapFile[i]->Split(',');
		HitObject ^hitobject = gcnew HitObject;
		hitobject->x = System::Convert::ToInt32(Line[0]);
		hitobject->y = System::Convert::ToInt32(Line[1]);
		hitobject->Time = System::Convert::ToInt32(Line[2]);
		hitobject->Type = System::Convert::ToInt32(Line[3]);

		if (hitobject->Type == 2 || hitobject->Type == 6 ||
			hitobject->Type == 21 || hitobject->Type == 22)	//Slider
		{
			//hitobject->SliderType = Line[5];
			hitobject->Repetition = System::Convert::ToInt32(Line[6]);
			hitobject->PixelLength = System::Convert::ToDouble(Line[7]->Replace(".", ","));
		}

		if(hitobject->Type == 8 || hitobject->Type == 12) //Spin
			hitobject->SpinEndTime = System::Convert::ToInt32(Line[5]);

		HitObjects->Add(hitobject);

		i++;
	}
}


int Beatmap::getError()
{
	return Error;
}

int Beatmap::getVersion()
{
	return MapVersion;
}

double Beatmap::getMapOverallDifficulty()
{
	return MapOverallDifficulty;
}

double Beatmap::getMapApproachRate()
{
	return MapApproachRate;
}

double Beatmap::getMapCircleSize()
{
	return MapCircleSize;
}

double Beatmap::getMapSliderMultiplier()
{
	return MapSliderMultiplier;
}

System::Collections::Generic::List<BPM^> ^Beatmap::getBPMs()
{
	return BPMs;
}


System::Collections::Generic::List<HitObject^> ^Beatmap::getHitObjects()
{
	return HitObjects;
}