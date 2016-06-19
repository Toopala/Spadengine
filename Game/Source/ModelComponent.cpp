#include "Core/Assert.h"
#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/RenderSystem.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Texture.h"
#include "Renderer/CubeMap.h"

namespace sge
{
	ModelComponent::ModelComponent(Entity* entity) :
		Component(entity), shininess(2.0f), glossyness(0.0f), myCube(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	void ModelComponent::update()
	{
	}

	void ModelComponent::setModelResource(sge::Handle <sge::ModelResource>* modelHandle)
	{
		this->modelHandle = modelHandle;
	}

	void ModelComponent::setShininess(float shine)
	{
		this->shininess = shine;
	}

	float ModelComponent::getShininess()
	{
		return shininess;
	}

	void ModelComponent::setGlossyness(float gloss)
	{
		this->glossyness = gloss;
	}

	float ModelComponent::getGlossyness()
	{
		return glossyness;
	}

	CubeMap* ModelComponent::getCubeMap()
	{
		return myCube;
	}

	void ModelComponent::setCubeMap(CubeMap* cube)
	{
		myCube = cube;
	}
}