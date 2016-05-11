#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/RenderSystem.h"
#include "Core/Assert.h"

namespace sge
{
	ModelComponent::ModelComponent(Entity* entity) :
		RenderComponent(entity), shininess(100.0f), diffTexture(nullptr), normTexture(nullptr), specTexture(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	void ModelComponent::update()
	{
	}

	void ModelComponent::render(GraphicsDevice* device)
	{
        SGE_ASSERT(renderer);

		renderer->renderModel(this);
	}

	void ModelComponent::setModelResource(sge::Handle <sge::ModelResource>* modelHandle)
	{
		this->modelHandle = modelHandle;
		diffTexture = modelHandle->getResource<ModelResource>()->getDiffuseTexture();
		normTexture = modelHandle->getResource<ModelResource>()->getNormalTexture();
		specTexture = modelHandle->getResource<ModelResource>()->getSpecularTexture();
	}

	void ModelComponent::setShininess(float shine)
	{
		this->shininess = shine;
	}

	float ModelComponent::getShininess()
	{
		return shininess;
	}
}