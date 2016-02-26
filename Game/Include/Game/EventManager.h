#pragma once
#include "SDL2/SDL.h"
#include "HID/KeyboardInput.h"
#include "HID/MouseInput.h"
#include "HID/GamepadInput.h"


// Manager for handling HID
namespace sge
{		
	class EventManager
		{
		public:
			EventManager(MouseInput* mouseInput);
			~EventManager();

			void update();

			bool userQuit();

		private:
			void processInput();
			bool quitState;

			//sge::KeyboardInput* keyboardInput;
			sge::MouseInput* mouseInput;
			sge::GamepadInput* gamepadInput;
			
	};
}