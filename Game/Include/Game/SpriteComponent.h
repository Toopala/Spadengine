#pragma once
#include "Game/RenderingComponent.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;
    class SpriteRenderingSystem;

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
		void setColor(const math::vec4& color);
        void setRenderingSystem(SpriteRenderingSystem* system);

        const math::vec4& getColor();
		
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

		TransformComponent* transform;
        SpriteRenderingSystem* renderingSystem;
	};
}
