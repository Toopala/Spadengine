#pragma once
#include "Game/RenderingComponent.h"

namespace sge
{
	class SpriteComponent : public RenderingComponent
	{
	public:
		SpriteComponent(Entity* ent);
		~SpriteComponent();
		void render(GraphicsDevice* device);

	private:
		sge::Buffer* vertexBuffer;
		sge::Texture* texture;
		sge::Shader* shader;
	};
}
