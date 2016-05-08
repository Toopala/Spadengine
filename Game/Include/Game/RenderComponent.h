#pragma once

#include <vector>

#include "Game/Component.h"
#include "Game/CameraComponent.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderCommand.h"

namespace sge
{
    class RenderSystem;

	class RenderComponent : public Component
	{
	public:
		RenderComponent(Entity* ent);
        virtual ~RenderComponent();
		
        void setRenderer(RenderSystem* renderer);

		RenderCommand key;
        virtual void render(GraphicsDevice* device) = 0;

    protected:
        RenderSystem* renderer;
	};
}

