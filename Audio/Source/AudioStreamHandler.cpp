#include "Audio/AudioStreamHandler.h"
#include <iostream>
#include <string.h> //memset
#include <Core/Assert.h> // assert
namespace sge
{
	AudioStreamHandler::AudioStreamHandler()
		:data()
	{
		Pa_Initialize();
		std::cout<< Pa_GetVersionText() << std::endl;
		PaError err;
		PaStreamParameters outputParameters;

		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.channelCount = CHANNEL_COUNT;		/*Stereo output*/
		outputParameters.sampleFormat = paInt32;
		outputParameters.suggestedLatency =Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(&stream,
			NO_INPUT,
			&outputParameters,
			SAMPLE_RATE,
			paFramesPerBufferUnspecified,
			paClipOff,
			&paStreamCallBack,
			this);

		if (err)
		{
			Pa_Terminate();

			std::stringstream error;
			error << "Unable to open stream for output. Portaudio error code: " << err;
            //std::abort();
            SGE_ASSERT(0 );
            //assert(0);
		}
	}

	AudioStreamHandler::~AudioStreamHandler()
	{
		Pa_CloseStream(stream);

		for (auto wrapper : data)
		{
			delete wrapper;
		}
		Pa_Terminate();
	}

	void AudioStreamHandler::processEvent(AudioEventType audioEventType, AudioFile *audioFile, bool loop)
	{
		switch (audioEventType) {
		case start:
			if (Pa_IsStreamStopped(stream))
			{
				Pa_StartStream(stream);
			}

			data.push_back(new Playback{
				audioFile,
				0,
				loop
			});

			break;
		case stop:
			Pa_StopStream(stream);
			for (auto instance : data)
			{
				delete instance;
			}
			data.clear();
			break;
		}
	}

	int AudioStreamHandler::paStreamCallBack(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		AudioStreamHandler *streamHandler = (AudioStreamHandler*)userData;

		unsigned long stereoFrameCount = frameCount * streamHandler->CHANNEL_COUNT;
		memset((int*)output, 0, stereoFrameCount * sizeof(int));

		if (streamHandler->data.size() > 0)
		{
			auto it = streamHandler->data.begin();
			while (it != streamHandler->data.end())
			{
				Playback *data = (*it);
				AudioFile *audioFile = data->audiofile;

				int *outputBuffer = new int[stereoFrameCount];
				int *bufferCursor = outputBuffer;

				unsigned int framesLeft = (unsigned int)frameCount;
				unsigned int framesRead;

				bool playbackEnded = false;
				while (framesLeft > 0)
				{
					sf_seek(audioFile->data, data->position, SEEK_SET);

					if (framesLeft > (audioFile->info.frames - data->position))
					{
						framesRead = (unsigned int)(audioFile->info.frames - data->position);
						if (data->loop)
						{
							data->position = 0;
						}
						else
						{
							playbackEnded = true;
							framesLeft = framesRead;
						}
					}
					else
					{
						framesRead = framesLeft;
						data->position += framesRead;
					}

					sf_readf_int(audioFile->data, bufferCursor, framesRead);

					bufferCursor += framesRead;

					framesLeft -= framesRead;
				}

				int * outputCursor = (int *)output;
				if (audioFile->info.channels == 1) {
					for (unsigned long i = 0; i < stereoFrameCount; ++i)
					{
						*outputCursor += (0.5 * outputBuffer[i]);
						++outputCursor;
						*outputCursor += (0.5 * outputBuffer[i]);
						++outputCursor;
					}
				}
				else
				{
					for (unsigned long i = 0; i < stereoFrameCount; ++i)
					{
						*outputCursor += (0.5 * outputBuffer[i]);
						++outputCursor;
					}
				}

				if (playbackEnded) {
					it = streamHandler->data.erase(it);
					delete data;
				}
				else
				{
					++it;
				}
			}
		}
		return paContinue;
	}
}
