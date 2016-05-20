#pragma once

#include "portaudio.h"
//#include "pa_asio.h"
#include "sndfile.h"
#include <stdio.h>
#include <vector>
#include <sstream>

namespace sge
{
	/** \brief Hold information about audio data. */
	struct AudioFile
	{
		SNDFILE *data;	/**< Pointer to audio data. */
		SF_INFO info;	/**< Passing data between the calling function and the library when opening a file for reading or writing. */
	};

	/** \brief Playback for audio. */
	struct Playback
	{
		AudioFile *audiofile; /**< Pointer to audio file. */
		int position;		  /**< Position of the sound. */
		bool loop;			  /**< Loop for the audio. */
	};

	/** \brief Audio event types. */
	enum AudioEventType
	{
		start,
		stop
	};

	/** \brief Class for handling all the audio data.
	*
	*	Class handles all the audiodata, initializes PortAudio and uses libsndfile for writing audio data.
	*	
	*/
	class AudioStreamHandler
	{
	public:
		/** \brief The default constructor.
		*
		*	Initializes the PortAudio and gives parameters to the PortAudio device.
		*/
		AudioStreamHandler();

		/** \brief The destructor.
		*
		*	Closes PortAudio stream, deletion of all the audio data and terminates the device.
		*/
		~AudioStreamHandler();

		/** \brief Process the audio events.
		*
		*	Handles all the audio events: start and stop.
		*	Use in Audio.cpp to play, loop or stop audio.
		*/
		void processEvent(AudioEventType audioEventType, AudioFile *audiofile = nullptr, bool loop = false);

		/** \brief PortAudio stream callback.
		*
		*	Consumes, process and generates audio.
		*
		*/
		static int paStreamCallBack(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	private:
		const int CHANNEL_COUNT = 2;						/**< Channel count. */
		const int SAMPLE_RATE = 44100;						/**< Sample rate (44100). */
		const PaStreamParameters *NO_INPUT = nullptr;		/**< Pointer to PortAudio stream parameters. */

		PaStream *stream;									/**< Pointer to stream. */
		std::vector<Playback*> data;						/**< Pointer to Playback vector. */
	};
}