#pragma once

#include "Core/Math.h"
#include "AudioContext.h"

namespace sge
{
	class AudioListener
	{
	public:

		// Change he global volume of all the sounds and music
		static void setGlobalVolume(float volume)
		{
			AudioContext::setGlobalVolume(volume);
		}

		// Get the current global volume
		static float getGlobalVolume()
		{
			return AudioContext::getGlobalVolume();
		}

		// Set the position of the new listener
		static void setPosition(const math::vec3 &position)
		{
			AudioContext::setPosition(position);
		}
		// Set the position of the listener
		static void setPosition(float x, float y, float z)
		{
			setPosition(math::vec3(x, y, z));
		}

		// Return position of the listener in the current scene
		static math::vec3 getPosition()
		{
			return AudioContext::getPosition();
		}

		// Set forward vector of the new listener in the scene
		static void setDirection(const math::vec3 &direction)
		{
			AudioContext::setDirection(direction);
		}

		// Set forward vector of the listener in the scene
		static void setDirection(float x, float y, float z)
		{
			setDirection(math::vec3(x, y, z));
		}

		// Return direction of the listener in the current scene
		static math::vec3 getDirection()
		{
			return AudioContext::getDirection();
		}

		// Set the new upward vector for listener in the scene
		static void setUpVector(const math::vec3 &upVector)
		{
			AudioContext::setUpVector(upVector);
		}
		
		// Set the upward vector for listener in the scene
		static void setUpVector(float x, float y, float z)
		{
			setUpVector(math::vec3(x, y, z));
		}

		// Return upvector of the listener in the current scene
		static math::vec3 getUpVector()
		{
			return AudioContext::getUpVector();
		}
	};
}