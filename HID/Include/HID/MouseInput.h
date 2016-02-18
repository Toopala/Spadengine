#pragma once

#include "SDL/SDL_mouse.h"
#include <unordered_map>

namespace sge
{
	class MouseInput
	{
		enum MouseButton : unsigned int
		{
			MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
			MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
			MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
			MOUSE_BUTTON_X1 = SDL_BUTTON_X1,
			MOUSE_BUTTON_X2 = SDL_BUTTON_X2
		};
		enum MouseWheel : int
		{
			MOUSE_WHEEL_UP = 1,
			MOUSE_WHEEL_DOWN = -1
		};

	public:
		MouseInput();
		~MouseInput();

		void update();

		bool buttonIsPressed(MouseButton button);
		bool buttonWasPressed(MouseButton button);
		bool buttonWasReleased(MouseButton button);
		bool mouseWheelWasMoved(MouseWheel direction);
		bool mouseWasMoved();
		void getRelativeMouseState(int* x, int*y);

	protected:
		void pressButton(unsigned int button);
		void releaseButton(unsigned int button);
		void setMousePosition(int x, int y);
		void moveMouseWheel(int y);

	private:
		bool buttonWasDown(unsigned int button);

		std::unordered_map<unsigned int, bool> buttonMap;
		std::unordered_map<unsigned int, bool> previousButtonMap;
		
		int mouseWheelYPosition = 0;
	};
}