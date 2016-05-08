#include "Game/RenderComponent.h"

namespace sge
{
	RenderComponent::RenderComponent(Entity* ent) : Component(ent)
	{
		// TODO do a proper calculation for depth, pipeline etc.
		// Maybe in the derived classes?
		// Needs more planning!
		key.bits = 0;
	}

	RenderComponent::~RenderComponent()
	{
	}

    void RenderComponent::setRenderer(RenderSystem* renderer)
    {
        this->renderer = renderer;
    }
}