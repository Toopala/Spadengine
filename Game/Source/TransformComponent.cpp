#include "Game/TransformComponent.h"
#include <iostream>
namespace sge
{

	TransformComponent::TransformComponent(Entity* ent) : Component(ent), angle(0.0f), position(0.0f), scale(1.0f), rotationVector(0.0f)
	{
		std::cout << "TransformComponent created." << std::endl;
	}


	TransformComponent::~TransformComponent()
	{
	}
}