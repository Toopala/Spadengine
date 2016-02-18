#pragma once
#include "SDL/SDL.h"
#include "HID/KeyboardInput.h"
#include "HID/MouseInput.h"


// Manager for handling HID
namespace sge
{		
	class EventManager
		{
		public:
			EventManager();
			~EventManager();

			void update();

			bool userQuit();

		private:
			void processInput();

			bool quitState;
	};
}