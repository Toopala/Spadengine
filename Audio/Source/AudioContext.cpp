#include "Audio/AudioContext.h"

namespace sge
{
	// Constructor
	AudioContext::AudioContext()
	{
		audioDevice = alcOpenDevice(NULL);

		if (audioDevice)
		{
			// Create context

			audioContext = alcCreateContext(audioDevice, NULL);
			
			if (audioContext)
			{
				// Set the current context
				alcMakeContextCurrent(audioContext);

				// Apply the listener properties
				float orientation[] = { 
					listenerDirection.x,
					listenerDirection.y,
					listenerDirection.z,
					listenerUpVector.x,
					listenerUpVector.y,
					listenerUpVector.z
				};
			}
		}
	}

	void AudioContext::alCheckError(const char* file, unsigned int line, const char* expression)
	{
		// Get the last error
		ALenum errorCode = alGetError();

		if (errorCode != AL_NO_ERROR)
		{
			std::string fileString = file;
			std::string error = "Unknown error";
			std::string description = "No description";

			// Decode the error code
			switch (errorCode)
			{
			case AL_INVALID_NAME:
			{
				error = "AL_INVALID_NAME";
				description = "A bad name (ID) has been specified.";
				break;
			}

			case AL_INVALID_ENUM:
			{
				error = "AL_INVALID_ENUM";
				description = "An unacceptable value has been specified for an enumerated argument.";
				break;
			}

			case AL_INVALID_VALUE:
			{
				error = "AL_INVALID_VALUE";
				description = "A numeric argument is out of range.";
				break;
			}

			case AL_INVALID_OPERATION:
			{
				error = "AL_INVALID_OPERATION";
				description = "The specified operation is not allowed in the current state.";
				break;
			}

			case AL_OUT_OF_MEMORY:
			{
				error = "AL_OUT_OF_MEMORY";
				description = "There is not enough memory left to execute the command.";
				break;
			}
			}
		}
	}
	
	// Destructor
	AudioContext::~AudioContext()
	{
		// Destroy context
		alcMakeContextCurrent(NULL);
		if (audioContext)
		{
			alcDestroyContext(audioContext);
		}

		// Destroy device
		if (audioDevice)
		{
			alcCloseDevice(audioDevice);
		}
	}
}