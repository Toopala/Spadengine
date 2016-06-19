#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Core/Assert.h"
#include "Game/RenderSystem.h"

namespace sge
{
    SpriteComponent::SpriteComponent(Entity* ent) :
        Component(ent),
        color(1.0f),
        texture(nullptr),
        pipeline(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	SpriteComponent::SpriteComponent(Entity* ent, sge::Texture* texture, const sge::math::vec4& col) : 
        Component(ent),
		color(col),
		texture(texture),
        pipeline(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::update()
	{
	}

	void SpriteComponent::setColor(const math::vec4& color)
	{
		this->color = color;
	}

    void SpriteComponent::setTexture(Texture* texture)
    {
        this->texture = texture;
    }

    void SpriteComponent::setPipeline(Pipeline* pipeline)
    {
        this->pipeline = pipeline;
    }

    const math::vec4& SpriteComponent::getColor()
    {
        return color;
    }

    Texture* SpriteComponent::getTexture()
    {
        return texture;
    }

    Pipeline* SpriteComponent::getPipeline()
    {
        return pipeline;
    }
}
