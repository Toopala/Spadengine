#pragma once

#include "Game/Component.h"
#include "Core/Math.h"
#include "Resources/ResourceManager.h"
#include "Resources/ModelResource.h"

namespace sge
{
    struct Texture;
    struct Pipeline;
    struct CubeMap;

	class TransformComponent;
	class ModelComponent : public Component
	{
	public:
		ModelComponent(Entity* entity);

		void update();

		void setModelResource(sge::Handle <sge::ModelResource>* modelHandle);

		void setPipeline(Pipeline* pipeline) { this->pipeline = pipeline; }

		Pipeline* getPipeline() { return pipeline; }

		ModelResource* getModelResource() { return modelHandle->getResource<ModelResource>(); }

        TransformComponent* transform;

		void setShininess(float shine);

		float getShininess();

		void setGlossyness(float gloss);

		float getGlossyness();

		CubeMap* getCubeMap();

		void setCubeMap(CubeMap* cube);

	private:
		
		CubeMap* myCube;
		sge::Handle <sge::ModelResource>* modelHandle;
		Pipeline* pipeline;
		float shininess;
		float glossyness;
	};
}