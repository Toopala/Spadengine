#include "Game/TransformComponent.h"
#include <iostream>
namespace sge
{
	TransformComponent::TransformComponent(Entity* ent) : 
        Component(ent),
        position(0.0f),
        scale(1.0f),
        rotationVector(0.0f, 1.0f, 0.0f),
        front(0.0f, 0.0f, 1.0f),
        up(0.0f, 1.0f, 0.0f),
        left(1.0f, 0.0f, 0.0f),
        angle(0.0f)
	{
	}
}