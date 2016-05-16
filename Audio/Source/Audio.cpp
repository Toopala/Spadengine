#include "Audio/Audio.h"

namespace sge
{
	Audio::Audio()
		: audioStreamHandler()
	{
		
	}
	Audio::~Audio()
	{
		for (auto entry : sounds)
		{
			sf_close(entry.second.data);
		}
	}

	void Audio::play(std::string fileName)
	{
		audioStreamHandler.processEvent(AudioEventType::start, &getAudio(fileName));
	}

	void Audio::loop(std::string fileName)
	{
		audioStreamHandler.processEvent(AudioEventType::start, &getAudio(fileName), true);
	}

	void Audio::stop()
	{
		audioStreamHandler.processEvent(AudioEventType::stop);
	}

	bool Audio::containsAudio(std::string fileName)
	{
		return sounds.find(fileName) != sounds.end();
	}

	AudioFile &Audio::getAudio(std::string fileName)
	{
		if (!containsAudio(fileName))
		{
			SF_INFO info;
			info.format = 0;
			SNDFILE *audioFile = sf_open(fileName.c_str(), SFM_READ, &info);

			AudioFile sound{
				audioFile,
				info
			};

			if (!audioFile)
			{
				std::stringstream err;
				err << "Unable to open audio file '"
					<< fileName << "'";
				throw err.str();
			}
			sounds[fileName] = sound;
		}
		return sounds[fileName];
	}
}