#pragma once

#include "Game/RenderingComponent.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;
	class ModelRenderingSystem;

	class ModelComponent : public RenderingComponent
	{
	public:
		ModelComponent(Entity* entity);

		void update();
		void render(GraphicsDevice* device);

		void setRenderingSystem(ModelRenderingSystem* system);
	private:
		TransformComponent* transform;
		ModelRenderingSystem* renderingSystem;
	};
}