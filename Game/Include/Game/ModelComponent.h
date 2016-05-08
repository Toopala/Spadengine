#pragma once

#include "Game/RenderComponent.h"
#include "Core/Math.h"
#include "Resources/ResourceManager.h"
#include "Resources/ModelResource.h"
#include "Renderer/Pipeline.h"

#include "Renderer/Texture.h"

namespace sge
{
	class TransformComponent;

	class ModelComponent : public RenderComponent
	{
	public:
		ModelComponent(Entity* entity);

		void update();
		void render(GraphicsDevice* device);

		void setModelResource(sge::Handle <sge::ModelResource>* modelHandle);

		void setPipeline(Pipeline* pipeline) { this->pipeline = pipeline; }

		Pipeline* getPipeline() { return pipeline; }

		ModelResource* getModelResource() { return modelHandle->getResource<ModelResource>(); }

        TransformComponent* transform;
	private:
		
		sge::Handle <sge::ModelResource>* modelHandle;
		Pipeline* pipeline;
	public:
		Texture* diffTexture;
		Texture* normTexture;
		Texture* specTexture;
	};
}