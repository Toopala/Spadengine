#include "Game/EventManager.h"

namespace sge
{
	EventManager::EventManager(MouseInput* mInput) : quitState(false), mouseInput(mInput)
	{

	}
	EventManager::~EventManager()
	{

	}

	void EventManager::update()
	{
		processInput();
	}

	bool EventManager::userQuit()
	{
		return false;
	}

	void EventManager::processInput()
	{
		static SDL_Event inputEvent;

		while (SDL_PollEvent(&inputEvent))
		{
			switch (inputEvent.type)
			{
				//Keyboard
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;

				//Mouse
			case SDL_MOUSEBUTTONDOWN:
				mouseInput->pressButton(inputEvent.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouseInput->releaseButton(inputEvent.button.button);
				break;
			case SDL_MOUSEMOTION:
				mouseInput->setMousePosition(inputEvent.motion.x, inputEvent.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				mouseInput->moveMouseWheel(inputEvent.wheel.y);
				break;

				//Gamepad
			case SDL_CONTROLLERBUTTONDOWN:
				break;
			case SDL_CONTROLLERBUTTONUP:
				break;
			case SDL_CONTROLLERAXISMOTION:
				break;
			case SDL_CONTROLLERDEVICEADDED:
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				break;

				//QUIT
			case SDL_QUIT:
				quitState = true;
				break;

				//WINDOW EVENTS
			case SDL_WINDOWEVENT:
				break;
			}
		}
	}
}
		