#pragma once

#include "Audio/AudioStreamHandler.h"
#include <string>
#include <sstream>
#include <map>
#include <sys/types.h>

namespace sge
{	
	class Audio
	{
	public:
		Audio();
		~Audio();

		void play(std::string soundFile);
		void loop(std::string soundFile);
		void stop();

		bool containsAudio(std::string fileName);
		AudioFile &getAudio(std::string fileName);

	private:
		std::map<std::string, AudioFile> sounds;
		AudioStreamHandler audioStreamHandler;
	};
}