#include "portaudio.h"
#include "pa_asio.h"
#include <stdio.h>
#define TABLE_SIZE 200

namespace sge
{	
	typedef struct
	{
		float sine[TABLE_SIZE];
		int left_phase;
		int right_phase;
		char message[20];
	}paTestData;

	class Audio
	{
	public:
		Audio();
		~Audio();
		typedef int paStreamCallBack(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
		static int paTestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
		static void streamFinished(void* userData);
		void initAudio();

	private:
	};
}