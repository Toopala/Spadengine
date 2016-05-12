#pragma once

#include "portaudio.h"
#include "pa_asio.h"
#include "sndfile.h"
#include <stdio.h>
#include <vector>
#include <sstream>

namespace sge
{
	struct AudioFile
	{
		SNDFILE *data;
		SF_INFO info;
	};

	struct Playback
	{
		AudioFile *audiofile;
		int position;
		bool loop;
	};

	enum AudioEventType
	{
		start,
		stop
	};

	class AudioStreamHandler
	{
	public:
		AudioStreamHandler();
		~AudioStreamHandler();

		void processEvent(AudioEventType audioEventType, AudioFile *audiofile = nullptr, bool loop = false);
		static int paStreamCallBack(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	private:
		const int CHANNEL_COUNT = 2;
		const int SAMPLE_RATE = 44000;
		const PaStreamParameters * NO_INPUT = nullptr;

		PaStream *stream;
		std::vector<Playback*> data;
	};
}