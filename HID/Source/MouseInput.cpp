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
		//update previousButtonMap
		for (auto& it : buttonMap)
		{
			previousButtonMap[it.first] = it.second;
		}

		//Reset mouse wheel position
		mouseWheelYPosition = 0;

		//Update prevMousePosition
		prevMousePosition.x = mousePosition.x;
		prevMousePosition.y = mousePosition.y;
	}

	bool MouseInput::buttonIsPressed(MouseButton button)
	{
		auto it = buttonMap.find(button);
		if (it != buttonMap.end())
		{
			return it->second;
		}
		return false;
	}

	bool MouseInput::buttonWasPressed(MouseButton button)
	{
		if (buttonIsPressed(button) == true && buttonWasDown(button) == false)
		{
			return true;
		}
		return false;
	}

	bool MouseInput::buttonWasReleased(MouseButton button)
	{
		if (buttonIsPressed(button) == false && buttonWasDown(button) == true)
		{
			return true;
		}
		return false;
	}

	bool MouseInput::mouseWheelWasMoved(MouseWheel direction)
	{
		if (direction < 0)
		{
			if (mouseWheelYPosition < 0)
			{
				return true;
			}
			return false;
		}
		else
		{
			if (mouseWheelYPosition > 0)
			{
				return true;
			}
			return false;
		}
	}

	bool MouseInput::mouseWasMoved()
	{
		if (mousePosition.x != prevMousePosition.x || mousePosition.y != prevMousePosition.y)
		{
			return true;
		}
		return false;
	}

	void MouseInput::getRelativeMouseState(int* x, int*y)
	{
		SDL_GetRelativeMouseState(x, y);
	}

	glm::ivec2 MouseInput::getMousePosition()
	{
		return mousePosition;
	}

	int MouseInput::getMouseXPosition()
	{
		return mousePosition.x;
	}

	int MouseInput::getMouseYPosition()
	{
		return mousePosition.y;
	}

	void MouseInput::enableRelativeMousePosition()
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void MouseInput::disableRelativeMousePosition()
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

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
		mousePosition.x = x;
		mousePosition.y = y;
	}

	void MouseInput::moveMouseWheel(int y)
	{
		mouseWheelYPosition += y;
	}

	bool MouseInput::buttonWasDown(unsigned int button)
	{
		auto it = previousButtonMap.find(button);
		if (it != previousButtonMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}