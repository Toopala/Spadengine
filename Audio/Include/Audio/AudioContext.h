#pragma once

//#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
#include <OpenAL/alut.h>
#include <iostream>
#pragma warning (disable : 4996)		// HAX
#define NUM_BUFFERS 7

namespace sge
{
	class Audio
	{
	public:
		Audio()
		{
			// OpenAL initialization
			//init();
		}

		// List of audio devices
		static void listAudioDevices(const ALCchar *devices);
		
		// AL format
		static inline ALenum toALFormat(short channels, short samples);


		// Context initialization
		void init();

		// Display errors
		ALvoid displayALError(ALbyte *szText, ALint errorCode);

		~Audio()
		{

		}

	private:
		ALint error;
		ALCcontext *context;
		ALCdevice *device;
		ALuint buffers[NUM_BUFFERS], *source;
		ALenum format;
		ALsizei size, freq;
		ALvoid *data;
		ALboolean loop;
	};
}