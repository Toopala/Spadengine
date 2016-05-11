#include "Game/LightComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Core/Assert.h"

namespace sge
{
	LightComponent::LightComponent(Entity* ent) : Component(ent)
	{
		transform = getParent()->getComponent<TransformComponent>();

		SGE_ASSERT(transform);
	}

	LightComponent::~LightComponent()
	{
	}

	void LightComponent::update()
	{
	}

	void LightComponent::setAsDirLight(DirLight* dirLight)
	{
		this->dirLight = dirLight;

		lightType = dirLightEnum;
	}

	void LightComponent::setAsPointLight(PointLight* pointLight)
	{
		this->pointLight = pointLight;

		lightType = pointLightEnum;
	}
}