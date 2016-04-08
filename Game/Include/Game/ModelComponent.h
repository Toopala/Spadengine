#pragma once

#include "Game/RenderingComponent.h"
#include "Core/Math.h"
#include "Resources/ResourceManager.h"
#include "Resources/ModelResource.h"
#include "Renderer/Pipeline.h"

#include "Renderer/Texture.h"

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

		void setModelResource(sge::Handle <sge::ModelResource>* modelHandle);

		void setPipeline(Pipeline* pipeline) { this->pipeline = pipeline; }

		Pipeline* getPipeline() { return pipeline; }

		ModelResource* getModelResource() { return modelHandle->getResource<ModelResource>(); }
	private:
		TransformComponent* transform;
		ModelRenderingSystem* renderingSystem;
		sge::Handle <sge::ModelResource>* modelHandle;
		Pipeline* pipeline;
	public:
		Texture* diffTexture;
		Texture* normTexture;
	};
}