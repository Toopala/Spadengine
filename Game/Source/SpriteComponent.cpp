#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Core/Assert.h"

namespace sge
{
    SpriteComponent::SpriteComponent(Entity* ent) :
        RenderingComponent(ent),
        color(1.0f),
        texture(nullptr),
        renderingSystem(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();
        key.fields.translucent = (color.a < 1.0f) ? 1 : 0;

		// We need transform!
		SGE_ASSERT(transform);
	}

	SpriteComponent::SpriteComponent(Entity* ent, sge::SpriteRenderingSystem* system, sge::Texture* texture, const sge::math::vec4& col) : 
		RenderingComponent(ent),
		color(col),
		texture(texture),
		renderingSystem(system)
	{
		transform = getParent()->getComponent<TransformComponent>();
        key.fields.translucent = (color.a < 1.0f) ? 1 : 0;

		// We need transform!
		SGE_ASSERT(transform);
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::render(GraphicsDevice* device)
	{
        SGE_ASSERT(renderingSystem);

        renderingSystem->renderSprite(this);
	}

	void SpriteComponent::update()
	{
        // TODO Does nothing... DO WE NEED UPDATE OR NOT??
	}

	void SpriteComponent::setColor(const math::vec4& color)
	{
		this->color = color;
	}

    void SpriteComponent::setTexture(Texture* texture)
    {
        this->texture = texture;
    }

    void SpriteComponent::setRenderingSystem(SpriteRenderingSystem* system)
    {
        renderingSystem = system;
    }

    const math::vec4& SpriteComponent::getColor()
    {
        return color;
    }

    Texture* SpriteComponent::getTexture()
    {
        return texture;
    }
}
