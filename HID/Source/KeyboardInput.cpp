#include "HID/KeyboardInput.h"

namespace sge
{
	KeyboardInput::KeyboardInput()
	{

	}
	KeyboardInput::~KeyboardInput()
	{

	}

	void KeyboardInput::update()
	{
		// update previousKeyMap from keyMap
		for (auto& it : keyMap)
		{
			previousKeyMap[it.first] = it.second;
		}
	}


	bool KeyboardInput::keyIsPressed(KeyboardKey keyID)
	{
		if (keyMap.size() != 0)
		{
			auto it = keyMap.find(keyID);
			if (it != keyMap.end())
			{
				return it->second;
			}
		}
		return false;
	}


	bool KeyboardInput::keyIsPressed(KeyboardKey keyID, KeyboardKey modID)
	{
		if (keyMap.size() != 0)
		{
			auto it = keyMap.find(keyID);
			if (it != keyMap.end() && it->second == true)
			{
				it = keyMap.find(modID);
				if (it != keyMap.end())
				{
					return it->second;
				}
			}
		}
		return false;
	}


	bool KeyboardInput::keyIsPressed(KeyboardKey keyID, KeyboardKey modID, KeyboardKey modID2)
	{
		return false;
		if (keyMap.size() != 0)
		{
			auto it = keyMap.find(keyID);
			if (it != keyMap.end() && it->second == true)
			{
				it = keyMap.find(modID);
				if (it != keyMap.end() && it->second == true)
				{
					it = keyMap.find(modID2);
					if (it != keyMap.end())
					{
						return it->second;
					}
				}
			}
		}
		return false;
	}


	bool KeyboardInput::keyWasPressed(KeyboardKey keyID)
	{
		if (keyIsPressed(keyID) == true && keyWasDown(keyID) == false)
		{
			return true;
		}
		return false;
	}


	bool KeyboardInput::keyWasPressed(KeyboardKey keyID, KeyboardKey modID)
	{
		if (keyIsPressed(keyID, modID) == true && keyWasDown(keyID, modID) == false)
		{
			return true;
		}
		return false;
	}


	bool KeyboardInput::keyWasPressed(KeyboardKey keyID, KeyboardKey modID, KeyboardKey modID2)
	{
		if (keyIsPressed(keyID, modID, modID2) == true && keyWasDown(keyID, modID, modID2) == false)
		{
			return true;
		}
		return false;
	}


	bool KeyboardInput::keyWasReleased(KeyboardKey keyID)
	{
		if (keyIsPressed(keyID) == false && keyWasDown(keyID) == true)
		{
			return true;
		}
		return false;
	}


	bool KeyboardInput::anyKeyIsPressed()
	{
		for (auto& it : keyMap)
		{
			if (it.second == true)
			{
				return true;
			}
		}
		return false;
	}


	// Private:
	void KeyboardInput::pressKey(unsigned int keyID)
	{
		keyMap[keyID] = true;
	}


	void KeyboardInput::releaseKey(unsigned int keyID)
	{
		keyMap[keyID] = false;
	}


	bool KeyboardInput::keyWasDown(unsigned int keyID)
	{
		auto it = previousKeyMap.find(keyID);
		if (it != previousKeyMap.end())
			return it->second;
		else
			return false;
	}


	bool KeyboardInput::keyWasDown(unsigned int keyID, unsigned int modID)
	{
		auto it = previousKeyMap.find(keyID);
		if (it != previousKeyMap.end() && it->second == true)
		{
			it = previousKeyMap.find(modID);
			if (it != previousKeyMap.end())
			{
				return it->second;
			}
		}
		return false;
	}


	bool KeyboardInput::keyWasDown(unsigned int keyID, unsigned int modID, unsigned int modID2)
	{
		auto it = previousKeyMap.find(keyID);
		if (it != previousKeyMap.end() && it->second == true)
		{
			it = previousKeyMap.find(modID);
			if (it != previousKeyMap.end() && it->second == true)
			{
				it = previousKeyMap.find(modID2);
				if (it != previousKeyMap.end())
				{
					return it->second;
				}
			}
		}
		return false;
	}
}