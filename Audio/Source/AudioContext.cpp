#include "Audio/AudioContext.h"

namespace sge
{
	void Audio::listAudioDevices(const ALCchar *devices)
	{
		const ALCchar *device = devices, *next = devices + 1;
		size_t len = 0;

		fprintf(stdout, "Devices list:\n");
		fprintf(stdout, "----------\n");
		while (device && *device != '\0' && next && *next != '\0') {
			fprintf(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		fprintf(stdout, "----------\n");
	}
	
	void Audio::init()
	{
		ALCdevice *device;
		device = alcOpenDevice(NULL);
		if (!device)
		{
			std::cout << "ERROR" << std::endl;
		}

		ALCcontext *context;

		context = alcCreateContext(device, NULL);
		if (!alcMakeContextCurrent(context))
		{
			std::cout << "OPENAL CONTEXT FAILED" << std::endl;
			alGetError();
		}
	}
}