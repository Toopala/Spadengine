#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/ModelRenderingSystem.h"
#include "Core/Assert.h"

namespace sge
{
	ModelComponent::ModelComponent(Entity* entity) :
		RenderingComponent(entity),
		renderingSystem(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	void ModelComponent::update()
	{
		key.fields.translucent = 0;
		key.fields.depth = static_cast<uint64>(transform->getPosition().z);
	}

	void ModelComponent::render(GraphicsDevice* device)
	{
		SGE_ASSERT(renderingSystem);

		renderingSystem->renderModel(this);
	}

	void ModelComponent::setRenderingSystem(ModelRenderingSystem* system)
	{
		renderingSystem = system;
	}

	void ModelComponent::setModelResource(sge::Handle <sge::ModelResource>* modelHandle)
	{
		this->modelHandle = modelHandle;
		diffTexture = modelHandle->getResource<ModelResource>()->getDiffuseTexture();
		normTexture = modelHandle->getResource<ModelResource>()->getNormalTexture();
		specTexture = modelHandle->getResource<ModelResource>()->getSpecularTexture();
	}
}