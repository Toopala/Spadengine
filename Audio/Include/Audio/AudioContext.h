#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <iostream>

namespace sge
{
	// Initialization
	class Audio
	{
	public:
		Audio()
		{
			init();

			// OpenAL testing
			ALboolean enumeration;
			enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
			if (enumeration == AL_FALSE)
			{
				std::cout << "Not supported.." << std::endl;
			}
			else
			{
				std::cout << "Supported.." << std::endl;
			}

		}

		// List of audio devices
		static void listAudioDevices(const ALCchar *devices);

		// Context initialization
		void init();

		~Audio()
		{

		}

	private:

	};
}