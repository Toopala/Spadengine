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
	
	ALenum Audio::toALFormat(short channels, short samples)
	{
		bool stereo = (channels > 1);
		
		switch (samples)
		{
		case 16:
			if (stereo)
			{
				return AL_FORMAT_STEREO16;
			}
			else
			{
				return AL_FORMAT_MONO16;
			}
		case 8:
			if (stereo)
			{
				return AL_FORMAT_STEREO8;
			}
			else
			{
				return AL_FORMAT_MONO8;
			}
		default:
			return -1;
		}
	}

	// Display errors
	ALvoid Audio::displayALError(ALbyte *szText, ALint errorCode)
	{
		printf("%s%s", szText, alGetString(errorCode));
	}

	// Init
	void Audio::init()
	{
		// Open AL init
		// Open device
		device = alcOpenDevice(NULL);

		// Creating context
		context = alcCreateContext(device, NULL);

		// Activating context
		alcMakeContextCurrent(context);

		// Setting listeners
		ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
		ALfloat	listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };		// Listener facing into the screen
		
		// Setting listener attributes
		alListenerfv(AL_POSITION, listenerPos);	// Position
		alListenerfv(AL_VELOCITY, listenerVel); // Velocity
		alListenerfv(AL_ORIENTATION, listenerOri); // Orientation
		alGenBuffers(NUM_BUFFERS, buffers);		// Generate buffers

		// Loading ahem_test.wav file
		alutLoadWAVFile("../Samples/Assets/Audio/ahem_test.wav", &format, &data, &size, &freq, &loop);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayALError("alutLoadWAVFile ahem_test.wav : ", error);
			// Delete buffers
			alDeleteBuffers(NUM_BUFFERS, buffers);
			return;
		}
		// Copy ahem_test.wav data into AL buffer 0
		alBufferData(buffers[NUM_BUFFERS], format, data, size, freq);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayALError("alBufferData buffer 0 : ", error);
			// Delete buffers
			alDeleteBuffers(NUM_BUFFERS, buffers);
			return;
		}
		// Unload ahem_test.wav
		alutUnloadWAV(format, data, size, freq);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayALError("alutUnloadWAV : ", error);
			// Delete buffers
			alDeleteBuffers(NUM_BUFFERS, buffers);
			return;
		}
		// Generate sources
		alGenSources(1, source);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayALError("alGenSources 1 : ", error);
			// Delete buffers
			alDeleteBuffers(NUM_BUFFERS, buffers);
			return;
		}

		// Attach buffer 0 to source
		alSourcei(source[0], AL_BUFFER, buffers[0]);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			displayALError("alSourcei AL_BUFFER 0 : ", error);
		}

		// Exit, clean up
		alDeleteBuffers(NUM_BUFFERS, buffers);
		context = alcGetCurrentContext();
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}