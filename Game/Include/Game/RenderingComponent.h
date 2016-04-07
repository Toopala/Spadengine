#pragma once
#include "Game/Component.h"
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
	};
}

