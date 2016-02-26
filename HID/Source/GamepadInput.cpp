#include "HID/GamepadInput.h"

// TODO: GAMEPAD COUTS TO DEBUG PROJECT...
#include <iostream>

namespace sge
{
	GamepadInput::GamepadInput()
	{

	}

	GamepadInput::~GamepadInput()
	{
		for (auto& it : gamepads)
		{
			delete it.second;
		}
	}

	bool GamepadInput::createGamepad(int newGPindex)
	{
		if (gamepadIsCreated(newGPindex))
		{
			std::cout << "Gamepad with index " << newGPindex << " has already been created!" << std::endl;
			return false;
		}

		joystickIndexMap[newGPindex] = SDL_GameControllerOpen(newGPindex);
		if (joystickIndexMap[newGPindex])
		{
			std::cout << "Created gamepad " << newGPindex;
			std::cout << "\n\tGamepad Name: " << SDL_GameControllerName(joystickIndexMap[newGPindex]) << std::endl;
			gamepads[newGPindex] = new GamepadMaps();
			return true;
		}
		else
		{
			std::cout << "Gamepad with index " << newGPindex << " couldn't be created!" << std::endl;
			return false;
		}
	}

	void GamepadInput::removeGamepad(int GPindex)
	{
		if (gamepadIsCreated(GPindex))
		{
			SDL_GameControllerClose(joystickIndexMap[GPindex]);
			delete gamepads[GPindex];
		}
	}

	bool GamepadInput::gamepadIsCreated(int GPindex)
	{
		if (joystickIndexMap[GPindex] != nullptr)
		{
			return true;
		}
		return false;
	}

	void GamepadInput::update()
	{
		//update previousMaps from keyMaps
		static auto gpIt = gamepads.begin();
		gpIt = gamepads.begin();
		while (gpIt != gamepads.end())
		{
			for (auto& it : gpIt->second->buttonMap)
			{
				gpIt->second->previousButtonMap[it.first] = it.second;
			}
			for (auto& it : gpIt->second->axisMap)
			{
				gpIt->second->previousAxisMap[it.first] = it.second;
			}
			for (auto& it : gpIt->second->xBallMap)
			{
				gpIt->second->previousXBallMap[it.first] = it.second;
			}
			for (auto& it : gpIt->second->yBallMap)
			{
				gpIt->second->previousYBallMap[it.first] = it.second;
			}
			for (auto& it : gpIt->second->hatMap)
			{
				gpIt->second->previousHatMap[it.first] = it.second;
			}
			gpIt++;
		}
	}

	int GamepadInput::getAxisValue(GamepadAxis buttonID, int GPindex)
	{
		if (gamepads[GPindex]->axisMap.size() != 0)
		{
			auto it = gamepads[GPindex]->axisMap.find(buttonID);
			if (it != gamepads[GPindex]->axisMap.end())
			{
				return it->second;
			}
		}
		return false;
	}

	bool GamepadInput::buttonIsPressed(GamepadButton buttonID, int GPindex)
	{
		if (gamepads[GPindex]->buttonMap.size() != 0)
		{
			auto it = gamepads[GPindex]->buttonMap.find(buttonID);
			if (it != gamepads[GPindex]->buttonMap.end())
			{
				return it->second;
			}
		}
		return false;
	}

	bool GamepadInput::buttonWasPressed(GamepadButton buttonID, int GPindex)
	{
		if (buttonIsPressed(buttonID, GPindex) && !buttonWasDown(buttonID, GPindex))
		{
			return true;
		}
		return false;
	}

	bool GamepadInput::buttonWasReleased(GamepadButton buttonID, int GPindex)
	{
		if (!buttonIsPressed(buttonID, GPindex) && buttonWasDown(buttonID, GPindex))
		{
			return true;
		}
		return false;
	}

	int GamepadInput::getBallXMotion(int GPindex, int ballIndex)
	{
		if (gamepads[GPindex]->xBallMap.size() != 0)
		{
			auto it = gamepads[GPindex]->xBallMap.find(ballIndex);
			if (it != gamepads[GPindex]->xBallMap.end())
			{
				return it->second;
			}
		}
		return false;
	}

	int GamepadInput::getBallYMotion(int GPindex, int ballIndex)
	{
		if (gamepads[GPindex]->yBallMap.size() != 0)
		{
			auto it = gamepads[GPindex]->yBallMap.find(ballIndex);
			if (it != gamepads[GPindex]->yBallMap.end())
			{
				return it->second;
			}
		}
		return false;
	}

	GamepadHatPosition GamepadInput::getHatPosition(int GPindex, int hatIndex)
	{
		if (gamepads[GPindex]->hatMap.size() != 0)
		{
			auto it = gamepads[GPindex]->hatMap.find(hatIndex);
			if (it != gamepads[GPindex]->hatMap.end())
			{
				return it->second;
			}
			else if (gamepadIsCreated(GPindex))
			{
				return GAMEPAD_HAT_CENTERED;
			}
		}
		return GAMEPAD_HAT_INVALID;
	}

	int GamepadInput::getNumGamepads()
	{
		return SDL_NumJoysticks();
	}

	void GamepadInput::setAxisControlDeadZone(short value, int GPindex)
	{
		axisDeadZoneMap[GPindex] = value;
	}

	short GamepadInput::getAxisControlDeadZone(int GPindex)
	{
		return axisDeadZoneMap[GPindex];
	}

	//Protected
	void GamepadInput::pressButton(unsigned int buttonID, int GPindex)
	{
		gamepads[GPindex]->buttonMap[buttonID] = true;
	}

	void GamepadInput::releaseButton(unsigned int buttonID, int GPindex)
	{
		gamepads[GPindex]->buttonMap[buttonID] = false;
	}

	void GamepadInput::addDevice(int GPindex)
	{
		createGamepad(GPindex);
	}

	void GamepadInput::removeDevice(int GPindex)
	{
		removeGamepad(GPindex);
	}

	void GamepadInput::hatMotion(int hatIndex, unsigned int hatPosition, int GPindex)
	{
		gamepads[GPindex]->hatMap[hatIndex] = (GamepadHatPosition)hatPosition;
	}

	void GamepadInput::ballMotion(int ballIndex, int xMovement, int yMovement, int GPindex)
	{
		gamepads[GPindex]->xBallMap[ballIndex] = (xMovement > 0) ? true : false;
		gamepads[GPindex]->yBallMap[ballIndex] = (yMovement > 0) ? true : false;
	}

	void GamepadInput::axisMotion(int axisIndex, int value, int GPindex)
	{
		gamepads[GPindex]->axisMap[axisIndex] = value;
	}

	//Private
	bool GamepadInput::buttonWasDown(GamepadButton buttonID, int GPindex)
	{
		auto it = gamepads[GPindex]->previousButtonMap.find(buttonID);
		if (it != gamepads[GPindex]->previousButtonMap.end())
		{
			return it->second;
		}
		return false;
	}
}