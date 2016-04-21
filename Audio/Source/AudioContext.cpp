#include "Audio/AudioContext.h"
#include <memory>

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

				alListenerf(AL_GAIN, listenerVolume * 0.01f);
				alListener3f(AL_POSITION, listenerPosition.x, listenerPosition.y, listenerPosition.z);
				alListenerfv(AL_ORIENTATION, orientation);
			}
			else
			{
				std::cout << "Audio context couldn't be created" << std::endl;
			}
		}
		else
		{
			std::cout << "NO AUDIO DEVICE FOUND!" << std::endl;
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

	// Creating a temporary audio device if there isn't already
	bool AudioContext::isExtensionSupported(const std::string &extension)
	{
		std::auto_ptr<AudioContext> device;
		if (!audioDevice)
		{
			device.reset(new AudioContext);
		}

		if ((extension.length() > 2) && (extension.substr(0, 3) == "ALC"))
		{
			return alcIsExtensionPresent(audioDevice, extension.c_str()) != AL_FALSE;
		}

		else
		{
			return alIsExtensionPresent(extension.c_str()) != AL_FALSE;
		}
	}
	
	int AudioContext::getFormatFromChannelCount(unsigned int channelCount)
	{
		std::auto_ptr<AudioContext> device;

		if (!audioDevice)
		{
			device.reset(new AudioContext);
		}

		int format = 0;

		switch (channelCount)
		{
		case 1:
			format = AL_FORMAT_MONO16;
			break;
		case 2:
			format = AL_FORMAT_STEREO16;
			break;
		case 3:
			format = alGetEnumValue("AL_FORMAT_QUARD16");
			break;
		case 4:
			format = alGetEnumValue("AL_FORMAT_51CHN16");
			break;
		case 5:
			format = alGetEnumValue("AL_FORMAT_61CHN16");
			break;
		case 6:
			format = alGetEnumValue("AL_FORMAT_71CHN16");
			break;
		default:
			format = 0;
			break;
		}

		return format;
	}

	void AudioContext::setGlobalVolume(float volume)
	{
		if (audioContext)
		{
			alListenerf(AL_GAIN, volume * 0.01f);
		}
		else
		{
			std::cout << "Audio context not found!" << std::endl;
		}

		listenerVolume = volume;
	}

	float AudioContext::getGlobalVolume()
	{
		return listenerVolume;
	}

	sge::math::vec3 AudioContext::getPosition()
	{
		return listenerPosition;
	}

	void AudioContext::setPosition(const sge::math::vec3 &direction)
	{
		if (audioContext)
		{
			float orientation[] = { direction.x, direction.y, direction.z, listenerUpVector.x, listenerUpVector.y, listenerUpVector.z };
			alListenerfv(AL_ORIENTATION, orientation);
		}

		listenerDirection = direction;
	}

	sge::math::vec3 AudioContext::getDirection()
	{
		return listenerDirection;
	}

	void AudioContext::setUpVector(const sge::math::vec3 &upVector)
	{
		if (audioContext)
		{
			float orientation[] = { listenerDirection.x, listenerDirection.y, listenerDirection.z, upVector.x, upVector.y, upVector.z };
			alListenerfv(AL_ORIENTATION, orientation);
		}
		
		listenerUpVector = upVector;
	}

	sge::math::vec3 AudioContext::getUpVector()
	{
		return listenerUpVector;
	}
}