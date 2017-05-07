#include "Sound.h"
#include "../imgui/imgui.h"

/* See the file "LICENSE" for the full license governing this code. */

  Sound::Sound()
{
}


Sound::~Sound()
{
}

bool Sound::Init(WCHAR* fileLocation)
{
	int default_device = -1; // Default Sounddevice
	int sampling_frequency = 44100; // Sample rate (Hz)
	BASS_Init(default_device, sampling_frequency, 0, 0, NULL);

	//	 Load music 
	
	if (!(channel = BASS_MusicLoad(FALSE, fileLocation, 0, 10, BASS_MUSIC_RAMPS | BASS_SAMPLE_LOOP | BASS_SAMPLE_MONO, 1)))	
	{

		int error = BASS_ErrorGetCode();
		if (!(channel = BASS_StreamCreateFile(FALSE, fileLocation, 0, 0, BASS_SAMPLE_LOOP) ))
		{
			return false; // Can't load the file
		}
	}

	// play music
	BASS_ChannelPlay(channel, FALSE);

	BASS_ChannelGetInfo(channel, &channelInformation); // get number of channels


	return false;
}

void Sound::setPause(bool isPause)
{
	if (isPause)
	{
		BASS_Pause();
	}
	else
	{
		BASS_ChannelPlay(channel, FALSE);

	}
}

SoundAttribute Sound::GetAttribute()
{
	SoundAttribute newAtts;


	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_CPU, &newAtts.cpuUssage);
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_FREQ, &newAtts.sameplRate);
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_VOL, &newAtts.volume);
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_PAN, &newAtts.pan);
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_EAXMIX, &newAtts.earMix);
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_SRC, &newAtts.quality);

	float f;
	BASS_ChannelGetAttribute(channel, BASS_ATTRIB_MUSIC_BPM, &f);

	ImGui::Text(" ffff %f", f);


	return newAtts;

}

void Sound::GUI_Menu(bool * is_open)
{

	if (*is_open == true)
	{

		// Create the window
		if (!ImGui::Begin("Music", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		float f;
		ImGui::Text("Sound Atriblues");
		SoundAttribute att =  GetAttribute();

		ImGui::Text("Volume: %f", att.volume);
		ImGui::Text("Cpu Ussage: %f", att.cpuUssage);
		ImGui::Text("Sample Rate: %f", att.sameplRate);
		ImGui::Text("Quality: %f", att.quality);
		ImGui::Text("Ear Mix: %f", att.earMix);
		ImGui::Text("Pan: %f", att.pan);
 
 		ImGui::Separator();

		DWORD level, left, right;
		level = BASS_ChannelGetLevel(channel);
		left = LOWORD(level);  
		right = HIWORD(level); 
		ImGui::Text("Left channel level : %f", (float)left);
		ImGui::Text("right channel level : %f",(float)right);
 
		BASS_CHANNELINFO channelInfo;

		BASS_ChannelGetInfo(channel,&channelInfo);

		ImGui::Text("number of channels %f", (float)channelInfo.chans);
		ImGui::Text("Frequancy %f", (float)channelInfo.freq);

 
		float data_FFT_256[128];
		BASS_ChannelGetData(channel, data_FFT_256, BASS_DATA_FFT256 | BASS_DATA_FLOAT);

		ImGui::PlotHistogram("Data FFt 256 ", data_FFT_256, IM_ARRAYSIZE(data_FFT_256), 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
		
		ImGui::SameLine();

		float data_FFT_256_complex[128*4];
		BASS_ChannelGetData(channel, data_FFT_256_complex, BASS_DATA_FFT256 | BASS_DATA_FFT_COMPLEX | BASS_DATA_FLOAT);
		 
		ImGui::PlotHistogram("Data FFt 256 complex ", data_FFT_256_complex, IM_ARRAYSIZE(data_FFT_256_complex), 0, NULL, -1.0f, 1.0f,ImVec2(0, 80));

 

		ImGui::SameLine();

		float data_FFT_256_individual[128 * 2];
		BASS_ChannelGetData(channel, data_FFT_256_individual, BASS_DATA_FFT256 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FLOAT);

		ImGui::PlotHistogram("Data FFt 256 individual ", data_FFT_256_individual, IM_ARRAYSIZE(data_FFT_256_individual), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));

		ImGui::SameLine();

		float data_FFT_256_removeDC[128 ];
		BASS_ChannelGetData(channel, data_FFT_256_removeDC, BASS_DATA_FFT256 | BASS_DATA_FFT_REMOVEDC | BASS_DATA_FLOAT);

		ImGui::PlotHistogram("Data FFt 256 no dc ", data_FFT_256_removeDC, IM_ARRAYSIZE(data_FFT_256_removeDC), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));

		ImGui::SameLine();

		float data_FFT_256_noWindow[128 ];
		BASS_ChannelGetData(channel, data_FFT_256_noWindow, BASS_DATA_FFT256 | BASS_DATA_FFT_NOWINDOW | BASS_DATA_FLOAT);

		ImGui::PlotHistogram("Data FFt 256 no dc ", data_FFT_256_noWindow, IM_ARRAYSIZE(data_FFT_256_noWindow), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));


		//float data_FFT_512[256];
		//BASS_ChannelGetData(channel, data_FFT_512, BASS_DATA_FFT512);

		//ImGui::PlotHistogram("Data FFt 512 ", data_FFT_512, IM_ARRAYSIZE(data_FFT_512), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));



		//float data_FFT_1024[512];
		//BASS_ChannelGetData(channel, data_FFT_1024, BASS_DATA_FFT1024);

		//ImGui::PlotHistogram("Data FFt 1024 ", data_FFT_1024, IM_ARRAYSIZE(data_FFT_1024), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));




		//float data_FFT_2048[1024];
		//BASS_ChannelGetData(channel, data_FFT_2048, BASS_DATA_FFT2048);

		//ImGui::PlotHistogram("Data FFt 2048 ", data_FFT_2048, IM_ARRAYSIZE(data_FFT_2048), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));





		//float data_FFT_4096[2048];
		//BASS_ChannelGetData(channel, data_FFT_4096, BASS_DATA_FFT4096);

		//ImGui::PlotHistogram("Data FFt 2096 ", data_FFT_4096, IM_ARRAYSIZE(data_FFT_4096), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));


		//float data_FFT_8192[4096];
		//BASS_ChannelGetData(channel, data_FFT_8192, BASS_DATA_FFT8192);

		//ImGui::PlotHistogram("Data FFt 8192 ", data_FFT_8192, IM_ARRAYSIZE(data_FFT_8192), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));



		//float data_FFT_16384[8192];
		//BASS_ChannelGetData(channel, data_FFT_16384, BASS_DATA_FFT16384);

		//ImGui::PlotHistogram("Data FFt 16384 ", data_FFT_16384, IM_ARRAYSIZE(data_FFT_16384), 0, NULL,-1.0f, 1.0f, ImVec2(0, 80));

		ImGui::SameLine();
		float data_FFT_16384_complex[8192*2];
		BASS_ChannelGetData(channel, data_FFT_16384_complex, BASS_DATA_FFT16384 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FLOAT);

		float test = 8000;

		ImGui::InputFloat("", &test);
		for (int i = 0; i <8192*2; ++i)
		{
			float size = (float)i / (8192*2 );
			// magnitude
			float mag = data_FFT_16384_complex[i];
			if (mag > 0.0002)
			{
				mag *= test * (1 + (size));
				data_FFT_16384_complex[i] = mag * 1/600;
			}
		}


		ImGui::PlotHistogram("Data FFt 16384 complex ", data_FFT_16384_complex, IM_ARRAYSIZE(data_FFT_16384_complex), 0, NULL,0.0f, 1.0f, ImVec2(0, 160));

		ImGui::End();

	}
}

float Sound::getLeftData()
{
	DWORD level, left, right;
	level = BASS_ChannelGetLevel(channel);
	left = LOWORD(level);
	right = HIWORD(level);
	return left;
}

float * Sound::getData(DWORD length, int size)
{

	float* data_FFT_data = new float[size];
	BASS_ChannelGetData(channel, data_FFT_data, length);



	return data_FFT_data;
}

 