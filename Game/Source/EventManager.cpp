#include "Game/EventManager.h"

namespace sge
{
	EventManager::EventManager() : quitState(false)
	{

	}
	EventManager::~EventManager()
	{

	}

	void EventManager::update()
	{
		processInput();
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
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEWHEEL:
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
		