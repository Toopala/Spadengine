#include "Audio/Audio.h"
#include <iostream>
#include <math.h>
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 64
#define PI 3.1415
#define NUM_SECONDS 1

namespace sge
{
	static paTestData data;
	Audio::Audio()
	{
		initAudio();
	}
	Audio::~Audio()
	{

	}

	void Audio::initAudio()
	{
		PaStreamParameters outputParameters;
		PaStream *stream;
		PaError err;
		paTestData data;

		std::cout << "PortAudio Test: output sine wave. SR = " << SAMPLE_RATE << "BufSize = " << FRAMES_PER_BUFFER << std::endl;

		// Initialise sinusoidal wavetable

		for (int i = 0; i < TABLE_SIZE; i++)
		{
			data.sine[i] = (float)sin(((double)i / (double)TABLE_SIZE)*PI * 2);
		}
		data.left_phase = data.right_phase = 0;

		err = Pa_Initialize();
		if (err != paNoError)
		{
			fprintf(stderr, "Pa_Initialize error: %s\n", Pa_GetErrorText(err));
		}

		// Default output device
		outputParameters.device = Pa_GetDefaultOutputDevice();
		if (outputParameters.device == paNoDevice)
		{
			std::cout << "Error: No default output device." << std::endl;
		}

		outputParameters.channelCount = 2;	// Stereo output
		outputParameters.sampleFormat = paFloat32;	// 32 bit floating point output
		outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowInputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(
			&stream,
			NULL, // No input
			&outputParameters,
			SAMPLE_RATE,
			FRAMES_PER_BUFFER,
			paClipOff,      //We won't output out of range samples so don't bother clipping them
			paTestCallback,
			&data);
		if (err != paNoError)
		{
			std::cout << "Error in opening stream" << Pa_GetErrorText(err) << std::endl;
		}

		// Get portaudio version
		std::cout << Pa_GetVersionText() << std::endl;

		/*err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, paTestCallback, &data);
		if (err != paNoError)
		{
			std::cout << "Error opening default stream: \n" << Pa_GetErrorText(err);
		}*/

		std::cout << data.message << " No message" << std::endl;
		
		err = Pa_SetStreamFinishedCallback(stream, &streamFinished);
		if (err != paNoError)
		{
			std::cout << "Error setting stream finished callback: " << Pa_GetErrorText(err) << std::endl;
		}

		err = Pa_StartStream(stream);
		if (err != paNoError)
		{
			std::cout << "Error starting stream: \n" << Pa_GetErrorText(err);
		}

		std::cout << "Paying for " << NUM_SECONDS << " seconds" << std::endl;
		Pa_Sleep(NUM_SECONDS * 1000);

		err = Pa_StopStream(stream);
		if (err != paNoError)
		{
			std::cout << Pa_GetErrorText(err);
		}

		err = Pa_CloseStream(stream);
		{
			std::cout << "Error closing stream: " << Pa_GetErrorText(err) << std::endl;
		}

		err = Pa_Terminate();
		if (err != paNoError)
		{
			std::cout << "PortAudio error: \n" << Pa_GetErrorText(err);
		}
	}

	void Audio::streamFinished(void* userData)
	{
		paTestData *data = (paTestData*)userData;
		std::cout << "Stream completed: " << data->message << std::endl;
	}

	int Audio::paTestCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
	{
		paTestData *data = (paTestData*)userData;
		float *out = (float*)outputBuffer;
		unsigned int i;
		(void)timeInfo;
		(void)statusFlags;
		(void)inputBuffer;

		for (size_t i = 0; i < framesPerBuffer; i++)
		{
			*out++ = data->sine[data->left_phase];  /* left */
			*out++ = data->sine[data->right_phase];  /* right */
			/* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
			data->left_phase += 1;
			/* When signal reaches top, drop back down. */
			if (data->left_phase >= TABLE_SIZE)
			{
				data->left_phase -= TABLE_SIZE;
			}
			/* higher pitch so we can distinguish left and right. */
			data->right_phase += 3;
			if (data->right_phase >= TABLE_SIZE)
			{
				data->right_phase -= TABLE_SIZE;
			}
		}
		return paContinue;
	}
}