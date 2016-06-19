#pragma once

#include "Core/Math.h"
#include "Game/Component.h"

#include <string>

namespace sge
{
	class TransformComponent;
    struct Pipeline;
    struct Texture;

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(Entity* ent);
		SpriteComponent(Entity* ent, sge::Texture* texture, const sge::math::vec4& col);
		~SpriteComponent();
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
