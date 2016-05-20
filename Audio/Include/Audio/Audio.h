#pragma once

#include "Audio/AudioStreamHandler.h"
#include <string>
#include <sstream>
#include <map>
#include <sys/types.h>

namespace sge
{	
	/** \brief Audio class, that handles audio play, loop and stop.
	*
	*	Engine uses PortAudio and libsndfile for audi I/O. Audio is only for stereo..
	*	Audio can play .wav/flac files.
	*/
	class Audio
	{
	public:
		/** \brief The default constructor. */
		Audio();
		/** \brief The destructor. */
		~Audio();

		/** \brief Plays single audio file for 1 time
		*
		*	Plays .wav/.flac file.
		*	\param string soundFile : Audio file name.
		*/
		void play(std::string soundFile);

		/** \brief Loops single audio file.
		*
		*	Loops .wav/flac file.
		*	\param string soundFile : Audio file name.
		*/
		void loop(std::string soundFile);

		/** \brief Stops audio
		*
		*	Stops all the audio files from playing or looping.
		*/
		void stop();

		/** \brief Contains audio
		*
		*	Checks, if the map contains any audio.
		*/
		bool containsAudio(std::string fileName);

		/** \brief Method, that loads audiofile
		*
		*	Loads audio file and gives error if it's unable to open audio file.
		*/
		AudioFile &getAudio(std::string fileName);

	private:
		std::map<std::string, AudioFile> sounds;	/**< Map that contains all the audio */
		AudioStreamHandler audioStreamHandler;		/**< Handler for all audio data. */
	};
}