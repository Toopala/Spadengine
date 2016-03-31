#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Core/Assert.h"

namespace sge
{
	SpriteComponent::SpriteComponent(Entity* ent) :
		RenderingComponent(ent),
		color(1.0f)
	{
		Entity* entity = getParent();


		transform = getParent()->getComponent<TransformComponent>();

		// We need transform!
		SGE_ASSERT(transform);
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::render(GraphicsDevice* device)
	{
		uniformData.MVP = VP * transform->getMatrix();

		uniformData.color = color;

		device->bindVertexBuffer(vertexBuffer);
		device->copyData(uniformBuffer, sizeof(uniformData), &uniformData);
		device->draw(3);
	}

	void SpriteComponent::update()
	{

	}

	void SpriteComponent::setVertexBuffer(sge::Buffer* buffer)
	{
		vertexBuffer = buffer;
	}

	void SpriteComponent::setUniformBuffer(sge::Buffer* buffer)
	{
		uniformBuffer = buffer;
	}

	void SpriteComponent::setTexture(sge::Texture* texture)
	{
		this->texture = texture;
	}

	void SpriteComponent::setPipeline(sge::Pipeline* pipeline)
	{
		this->pipeline = pipeline;
	}

	void SpriteComponent::setVP(const math::mat4& VP)
	{
		this->VP = VP;
	}

	void SpriteComponent::setColor(const math::vec4& color)
	{
		this->color = color;
	}
}
