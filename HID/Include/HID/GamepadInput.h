#pragma once
#include "SDL2/SDL_gamecontroller.h"
#include <unordered_map>

namespace sge
{

	const float AXIS_MAX = 32767.0f;

	enum GamepadButton
	{
		GAMEPAD_BUTTON_INVALID = -1,
		GAMEPAD_BUTTON_A,
		GAMEPAD_BUTTON_B,
		GAMEPAD_BUTTON_X,
		GAMEPAD_BUTTON_Y,
		GAMEPAD_BUTTON_BACK,
		GAMEPAD_BUTTON_GUIDE,
		GAMEPAD_BUTTON_START,
		GAMEPAD_BUTTON_LEFTSTICK,
		GAMEPAD_BUTTON_RIGHTSTICK,
		GAMEPAD_BUTTON_LEFTSHOULDER,
		GAMEPAD_BUTTON_RIGHTSHOULDER,
		GAMEPAD_BUTTON_DPAD_UP,
		GAMEPAD_BUTTON_DPAD_DOWN,
		GAMEPAD_BUTTON_DPAD_LEFT,
		GAMEPAD_BUTTON_DPAD_RIGHT,
		GAMEPAD_BUTTON_MAX
	};

	enum GamepadAxis
	{
		GAMEPAD_AXIS_INVALID = -1,
		GAMEPAD_AXIS_LEFTX,
		GAMEPAD_AXIS_LEFTY,
		GAMEPAD_AXIS_RIGHTX,
		GAMEPAD_AXIS_RIGHTY,
		GAMEPAD_AXIS_TRIGGERLEFT,
		GAMEPAD_AXIS_TRIGGERRIGHT,
		GAMEPAD_AXIS_MAX
	};

	enum GamepadHatPosition
	{
		GAMEPAD_HAT_INVALID = -1,
		GAMEPAD_HAT_CENTERED = 0x00,
		GAMEPAD_HAT_UP = 0x01,
		GAMEPAD_HAT_RIGHT = 0x02,
		GAMEPAD_HAT_DOWN = 0x04,
		GAMEPAD_HAT_LEFT = 0x08,
		GAMEPAD_HAT_RIGHTUP = (GAMEPAD_HAT_RIGHT | GAMEPAD_HAT_UP),
		GAMEPAD_HAT_RIGHTDOWN = (GAMEPAD_HAT_RIGHT | GAMEPAD_HAT_DOWN),
		GAMEPAD_HAT_LEFTUP = (GAMEPAD_HAT_LEFT | GAMEPAD_HAT_UP),
		GAMEPAD_HAT_LEFTDOWN = (GAMEPAD_HAT_LEFT | GAMEPAD_HAT_DOWN),
	};

	class GamepadInput
	{
	public:
		GamepadInput();
		~GamepadInput();

		void update();

		int getAxisValue(GamepadAxis buttonID, int GPindex);
		bool buttonIsPressed(GamepadButton buttonID, int GPindex);
		bool buttonWasPressed(GamepadButton buttonID, int GPindex);
		bool buttonWasReleased(GamepadButton buttonID, int GPindex);
		int getBallXMotion(int GPindex, int ballIndex);
		int getBallYMotion(int GPindex, int ballIndex);
		GamepadHatPosition getHatPosition(int GPindex, int hatIndex);

		int getNumGamepads();

		void setAxisControlDeadZone(short value, int GPindex);
		short getAxisControlDeadZone(int GPindex);


		void pressButton(unsigned int buttonID, int GPindex);
		void releaseButton(unsigned int buttonID, int GPindex);
		void addDevice(int GPindex);
		void removeDevice(int GPindex);
		void hatMotion(int hatIndex, unsigned int hatPosition, int GPindex);
		void ballMotion(int ballIndex, int xMovement, int yMovement, int GPindex);
		void axisMotion(int axisIndex, int value, int GPindex);

	private:
		bool createGamepad(int newGPindex); //returns true if successful
		void removeGamepad(int GPindex);
		bool gamepadIsCreated(int GPindex);
		bool buttonWasDown(GamepadButton buttonID, int GPindex);

		struct GamepadMaps
		{
			std::unordered_map<unsigned int, bool> buttonMap;
			std::unordered_map<unsigned int, bool> previousButtonMap;
			std::unordered_map<unsigned int, int> axisMap;
			std::unordered_map<unsigned int, int> previousAxisMap;
			std::unordered_map<unsigned int, bool> xBallMap;
			std::unordered_map<unsigned int, bool> previousXBallMap;
			std::unordered_map<unsigned int, bool> yBallMap;
			std::unordered_map<unsigned int, bool> previousYBallMap;
			std::unordered_map<unsigned int, GamepadHatPosition> hatMap;
			std::unordered_map<unsigned int, GamepadHatPosition> previousHatMap;
		};

		std::unordered_map<int, GamepadMaps*> gamepads;

		std::unordered_map<int, SDL_GameController*> joystickIndexMap;
		std::unordered_map<int, short> axisDeadZoneMap;
	};
}