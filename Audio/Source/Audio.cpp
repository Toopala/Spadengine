#include "Audio/Audio.h"
#include <iostream>
#define SAMPLE_RATE 44100

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
		PaError err = Pa_Initialize();
		if (err != paNoError)
		{
			fprintf(stderr, "Pa_Initialize error: %s\n", Pa_GetErrorText(err));
		}

		// Get portaudio version
		std::cout << Pa_GetVersionText() << std::endl;

		PaStream *stream;

		err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, paTestCallback, &data);
		if (err != paNoError)
		{
			std::cout << "Error opening default stream: \n" << Pa_GetErrorText(err);
		}

		err = Pa_StartStream(&stream);
		if (err != paNoError)
		{
			std::cout << "Error starting stream: \n" << Pa_GetErrorText(err);
		}

		err = Pa_StopStream(&stream);
		if (err != paNoError)
		{
			std::cout << Pa_GetErrorText(err);
		}

		err = Pa_Terminate();
		if (err != paNoError)
		{
			std::cout << "PortAudio error: \n" << Pa_GetErrorText(err);
		}
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
		(void)inputBuffer;

		for (size_t i = 0; i < framesPerBuffer; i++)
		{
			*out++ = data->left_phase;  /* left */
			*out++ = data->right_phase;  /* right */
			/* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
			data->left_phase += 0.01f;
			/* When signal reaches top, drop back down. */
			if (data->left_phase >= 1.0f) data->left_phase -= 2.0f;
			/* higher pitch so we can distinguish left and right. */
			data->right_phase += 0.03f;
			if (data->right_phase >= 1.0f) data->right_phase -= 2.0f;
		}
		return 0;
	}
}