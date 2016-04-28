#include "Audio/Audio.h"
#include <iostream>

namespace sge
{
	Audio::Audio()
	{

		PaError err = Pa_Initialize();
		if (err != paNoError)
		{
			fprintf(stderr, "Pa_Initialize error: %s\n", Pa_GetErrorText(err));
		}
		
		std::cout << Pa_GetVersionText() << std::endl;

		err = Pa_Terminate();
		if (err != paNoError)
		{
			printf("PortAUdio error: %s\n", Pa_GetErrorText(err));
		}
	}
}