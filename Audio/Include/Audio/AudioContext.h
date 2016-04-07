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
			ALCdevice *device;
			device = alcOpenDevice(NULL);
			if (!device)
			{ 
				std::cout << "ERROR";
			}

			// OpenAL testing
			ALboolean enumeration;
			enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
			if (enumeration == AL_FALSE)
			{
				std::cout << "Something.." << std::endl;
			}
		}
		~Audio()
		{

		}

	private:

	};
}