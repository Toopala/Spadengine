#include "Game/TransformComponent.h"
#include <iostream>
namespace sge
{

	TransformComponent::TransformComponent(Entity* ent) : Component(ent)
	{
		std::cout << "TransformComponent created." << std::endl;
	}


	TransformComponent::~TransformComponent()
	{
	}

}