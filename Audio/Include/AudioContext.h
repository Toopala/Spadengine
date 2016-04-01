#include <al.h>
#include <alc.h>
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
		}
		~Audio()
		{

		}

	private:

	};
}