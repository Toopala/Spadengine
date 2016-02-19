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

	bool MouseInput::buttonIsPressed(MouseButton button)
	{

	}

	bool MouseInput::buttonWasReleased(MouseButton button)
	{

	}

	bool MouseInput::buttonWasReleased(MouseButton button)
	{

	}

	bool MouseInput::mouseWheelWasMoved(MouseWheel direction)
	{

	}

	bool MouseInput::mouseWasMoved()
	{

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