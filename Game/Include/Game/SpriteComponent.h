#pragma once
#include "Game/RenderComponent.h"
#include "Core/Math.h"

#include <string>

namespace sge
{
	class TransformComponent;
    class RenderSystem;
    struct Pipeline;

	class SpriteComponent : public RenderComponent
	{
	public:
		SpriteComponent(Entity* ent);
		SpriteComponent(Entity* ent, sge::Texture* texture, const sge::math::vec4& col);
		~SpriteComponent();
		void render(GraphicsDevice* device);
		void update();

		void setTexture(Texture* texture);
		void setColor(const math::vec4& color);
        void setPipeline(Pipeline* pipeline);

        const math::vec4& getColor();
        Texture* getTexture();
        Pipeline* getPipeline();
		
        TransformComponent* transform;
	private:
        Texture* texture;
		math::vec4 color;
        Pipeline* pipeline;
	};
}
