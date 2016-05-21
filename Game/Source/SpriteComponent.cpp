#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Core/Assert.h"
#include "Game/RenderSystem.h"

namespace sge
{
    SpriteComponent::SpriteComponent(Entity* ent) :
        RenderComponent(ent),
        color(1.0f),
        texture(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();
        key.fields.translucent = (color.a < 1.0f) ? 1 : 0;

		SGE_ASSERT(transform);
	}

	SpriteComponent::SpriteComponent(Entity* ent, sge::Texture* texture, const sge::math::vec4& col) : 
		RenderComponent(ent),
		color(col),
		texture(texture)
	{
		transform = getParent()->getComponent<TransformComponent>();
        key.fields.translucent = (color.a < 1.0f) ? 1 : 0;

		SGE_ASSERT(transform);
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::render(GraphicsDevice* device)
	{
        SGE_ASSERT(renderer);

        renderer->renderSprite(this);
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
