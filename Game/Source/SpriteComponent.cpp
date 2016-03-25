#include "Game/SpriteComponent.h"

namespace sge
{
	SpriteComponent::SpriteComponent(Entity* ent) :
		RenderingComponent(ent),
		position(0.0f),
		scale(1.0f),
		color(1.0f),
		rotation(0.0f)
	{
	}

	SpriteComponent::~SpriteComponent()
	{
	}

	void SpriteComponent::render(GraphicsDevice* device)
	{
		uniformData.MVP = VP * 
			math::translate(math::mat4(1.0f), position) * 
			math::rotate(math::mat4(1.0f), rotation, math::vec3(0.0f, 0.0f, 1.0f)) *
			math::scale(math::mat4(1.0f), scale);

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

	void SpriteComponent::setPosition(const math::vec3& position)
	{
		this->position = position;
	}

	void SpriteComponent::setScale(const math::vec3& scale)
	{
		this->scale = scale;
	}

	void SpriteComponent::setColor(const math::vec4& color)
	{
		this->color = color;
	}

	void SpriteComponent::setRotation(float rotation)
	{
		this->rotation = rotation;
	}
}
