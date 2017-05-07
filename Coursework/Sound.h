#pragma once
#include <string>
#include "bass.h"
/* See the file "LICENSE" for the full license governing this code. */


struct SoundAttribute {

	float earMix;
	float bitRate;
	float cpuUssage;
	float sameplRate;
	float pan;
	float quality;
	float volume;
};
class Sound
{
public:
	Sound();
	~Sound();

	/*
	
	@param		fileLocation	The resourse localtion for audio to be loaded
	@return		If sound has been sucefully loaded
	*/
	bool Init(WCHAR* fileLocation);

	/*
	Sets if music is paused or not 
	*/
	void setPause(bool isPause);

	SoundAttribute GetAttribute();

	void GUI_Menu(bool* is_open);

	float getLeftData();

 	float* getData(DWORD length, int size);

private:

	bool isPause;
	BASS_CHANNELINFO channelInformation;
	DWORD channel;

};

 