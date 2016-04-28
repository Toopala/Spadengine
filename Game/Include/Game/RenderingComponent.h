#pragma once

#include <vector>

#include "Game/Component.h"
#include "Game/CameraComponent.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderCommand.h"

namespace sge
{
	class RenderingComponent : public Component
	{
	public:
		RenderingComponent(Entity* ent);
		virtual ~RenderingComponent();
		virtual void render(GraphicsDevice* device) = 0;

		RenderCommand key;
        std::vector<CameraComponent*> cameras;
	};
}

