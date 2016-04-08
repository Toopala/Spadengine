#pragma once
#include "Game/RenderingComponent.h"
#include "Core/Math.h"

#include <string>

namespace sge
{
	class TransformComponent;
    class SpriteRenderingSystem;

	class SpriteComponent : public RenderingComponent
	{
	public:
		SpriteComponent(Entity* ent);
		SpriteComponent(Entity* ent, sge::SpriteRenderingSystem* system, sge::Texture* texture, const sge::math::vec4& col);
		~SpriteComponent();
		void render(GraphicsDevice* device);
		void update();

		void setTexture(Texture* texture);
		void setColor(const math::vec4& color);
        void setRenderingSystem(SpriteRenderingSystem* system);

        const math::vec4& getColor();
        Texture* getTexture();
		
	private:
        Texture* texture;
		math::vec4 color;

		TransformComponent* transform;
        SpriteRenderingSystem* renderingSystem;
	};
}
