#include "Game/RenderingComponent.h"

namespace sge
{
	RenderingComponent::RenderingComponent(Entity* ent) : Component(ent)
	{
		// TODO do a proper calculation for depth, pipeline etc.
		// Maybe in the derived classes?
		// Needs more planning!
		key.bits = 1;
	}

	RenderingComponent::~RenderingComponent()
	{
	}
}