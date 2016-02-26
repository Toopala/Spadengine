#include "Game/EventManager.h"

namespace sge
{
	EventManager::EventManager(MouseInput* mInput, KeyboardInput* kbInput, GamepadInput* gpInput)
		: quitState(false), mouseInput(mInput), keyboardInput(kbInput), gamepadInput(gpInput)
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
		return quitState;
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
				keyboardInput->pressKey(inputEvent.button.button);
				break;
			case SDL_KEYUP:
				keyboardInput->releaseKey(inputEvent.button.which);
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
				gamepadInput->pressButton(inputEvent.cbutton.button, inputEvent.cbutton.which);
				break;
			case SDL_CONTROLLERBUTTONUP:
				gamepadInput->releaseButton(inputEvent.cbutton.button, inputEvent.cbutton.which);
				break;
			case SDL_CONTROLLERAXISMOTION:
				gamepadInput->axisMotion(inputEvent.caxis.axis, inputEvent.caxis.value, inputEvent.caxis.which);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				gamepadInput->addDevice(inputEvent.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				gamepadInput->removeDevice(inputEvent.cdevice.which);
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
		