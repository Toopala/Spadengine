#include "HID/MouseInput.h"

namespace sge
{
	MouseInput::MouseInput()
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	MouseInput::~MouseInput()
	{
	}

	void MouseInput::update()
	{

	}

	void MouseInput::getRelativeMouseState(int* x, int*y)
	{
		SDL_GetRelativeMouseState(x, y);
	}

	//Private:
	void MouseInput::pressButton(unsigned int button)
	{
		buttonMap[button] = true;
	}


	void MouseInput::releaseButton(unsigned int button)
	{
		buttonMap[button] = false;
	}


	void MouseInput::setMousePosition(int x, int y)
	{
		
	}


	bool MouseInput::buttonWasDown(unsigned int button)
	{
		auto it = previousButtonMap.find(button);
		if (it != previousButtonMap.end())
			return it->second;
		else
			return false;
	}


	void MouseInput::moveMouseWheel(int y)
	{
		mouseWheelYPosition += y;
	}
}