#include "Game/SpriteRenderingSystem.h"
#include "Renderer/GraphicsDevice.h"

namespace sge
{
	void SpriteRenderingSystem::update()
	{
	}

	void SpriteRenderingSystem::addComponent(Component* component)
	{
		SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(component);

		if (spriteComponent)
		{
			components.push_back(spriteComponent);
		}
	}
}