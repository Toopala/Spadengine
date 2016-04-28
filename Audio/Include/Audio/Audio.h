#include "portaudio.h"
#include "pa_asio.h"
#include <stdio.h>

namespace sge
{	
	typedef struct
	{
		float left_phase;
		float right_phase;
	}paTestData;

	class Audio
	{
	public:
		Audio();
		~Audio();
		typedef int paStreamCallBack(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
		static int paTestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

		void initAudio();

	private:
	};
}