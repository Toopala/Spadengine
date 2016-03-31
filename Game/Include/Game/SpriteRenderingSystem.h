#pragma once

#include "Game/System.h"
#include "Game/SpriteComponent.h"
#include <vector>

namespace sge
{
	class GraphicsDevice;

	class SpriteRenderingSystem : public System
	{
	public:
		void update();
		void addComponent(Component* component);

	private:
		std::vector<SpriteComponent*> components;
	};
}