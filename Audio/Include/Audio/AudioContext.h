#pragma once

#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "Core/Math.h"
#include "Core/Assert.h"

namespace
{
	ALCdevice*  audioDevice = NULL;
	ALCcontext* audioContext = NULL;

	float listenerVolume = 100.f;
	sge::math::vec3 listenerPosition(0.0f, 0.f, 0.f);
	sge::math::vec3 listenerDirection(0.0f, 0.0f, -1.0f);
	sge::math::vec3 listenerUpVector(0.0f, 1.0f, 0.0f);
}

namespace sge
{
	class AudioContext
	{
	public:
		// Constructor
		AudioContext();
		// Destructor
		~AudioContext();

		// Check if an OpenAL extension is supported
		static bool isExtensionSupported(const std::string& extension);

		// Check if OpenAL matches the given number of channels
		static int getFormatFromChannelCount(unsigned int channelCount);

		// Change the global volume of all the sounds and musics
		static void setGlobalVolume(float volume);

		// Get the current value of the global volume
		static float getGlobalVolume();

		// Set position of the listener in the scene
		// Default position is the (0, 0, 0).
		static void setPosition(const sge::math::vec3 &position);

		// Get the current position of the listener in the scene
		static sge::math::vec3 getPosition();

		// Set the forward vector of the listener in the scene
		static void setDirection(const sge::math::vec3 &direction);

		// Get the current forward vector of the listener in the scene
		static sge::math::vec3 getDirection();

		// Set the upward vector of the listener in the scene
		static void setUpVector(const sge::math::vec3 &upVector);

		// Get the current upward vector of the listener in the scene
		static sge::math::vec3 getUpVector();

		// Checking for OpenAL errors
		void alCheckError(const char* file, unsigned int line, const char* expression);

	private:
	};
}