#pragma once
#include "Game/RenderingComponent.h"
#include "Core/Math.h"

namespace sge
{
	class SpriteComponent : public RenderingComponent
	{
	public:
		SpriteComponent(Entity* ent);
		~SpriteComponent();
		void render(GraphicsDevice* device);
		void update();

		void setVertexBuffer(sge::Buffer* buffer);
		void setUniformBuffer(sge::Buffer* buffer);
		void setTexture(sge::Texture* texture);
		void setPipeline(sge::Pipeline* pipeline);
		void setVP(const math::mat4& VP);

		void setPosition(const math::vec3& position);
		void setScale(const math::vec3& scale);
		void setColor(const math::vec4& color);
		void setRotation(float rotation);
		
	private:
		sge::Buffer* vertexBuffer;
		sge::Buffer* uniformBuffer;
		sge::Texture* texture;
		sge::Pipeline* pipeline;

		struct UniformData
		{
			math::mat4 MVP;
			math::vec4 color;
		} uniformData;

		math::mat4 VP;
		math::vec4 color;
		math::vec3 position;
		math::vec3 scale;
		float rotation;
	};
}
