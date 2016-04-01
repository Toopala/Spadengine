#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/SpriteRenderingSystem.h"
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
        renderingSystem->renderSprite(this);
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

    void SpriteComponent::setRenderingSystem(SpriteRenderingSystem* system)
    {
        renderingSystem = system;
    }

    const math::vec4& SpriteComponent::getColor()
    {
        return color;
    }
}
