#pragma once
#include "Game/Component.h"
#include "SDL/SDL.h"

namespace sge
{ 
	class InputComponent : public Component
	{
	public:
		InputComponent(Entity* ent);
		~InputComponent();

		void update();
	private:
		SDL_Event event;
		Entity* parent;
	};
}
