#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/ModelRenderingSystem.h"
#include "Core/Assert.h"

namespace sge
{
	ModelComponent::ModelComponent(Entity* entity) :
		RenderComponent(entity),
		RenderSystem(nullptr)
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
		SGE_ASSERT(RenderSystem);

		RenderSystem->renderModel(this);
	}

	void ModelComponent::setRenderSystem(ModelRenderingSystem* system)
	{
		RenderSystem = system;
	}

	void ModelComponent::setModelResource(sge::Handle <sge::ModelResource>* modelHandle)
	{
		this->modelHandle = modelHandle;
		diffTexture = modelHandle->getResource<ModelResource>()->getDiffuseTexture();
		normTexture = modelHandle->getResource<ModelResource>()->getNormalTexture();
		specTexture = modelHandle->getResource<ModelResource>()->getSpecularTexture();
	}
}