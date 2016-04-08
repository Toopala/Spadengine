#include "Game/TransformComponent.h"
#include <iostream>
namespace sge
{

	TransformComponent::TransformComponent(Entity* ent) : Component(ent), angle(0.0f), position(0.0f), scale(1.0f), rotationVector(0.0f)
	{
		std::cout << "TransformComponent created." << std::endl;
	}

	TransformComponent::TransformComponent(Entity* ent, const sge::math::vec3& position, const sge::math::vec3& scale, float rotation) :
		Component(ent), angle(0.0f), rotationVector(rotation), scale(scale), position(position)
	{

	}

	TransformComponent::~TransformComponent()
	{
	}
}