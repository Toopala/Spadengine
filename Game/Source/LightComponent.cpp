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

	void LightComponent::setAsDirLight(sge::math::vec3 direction, sge::math::vec3 ambient, sge::math::vec3 diffuse, sge::math::vec3 specular)
	{
		this->direction = sge::math::vec4(direction, 1.0f);
		this->direction = sge::math::vec4(ambient, 1.0f);
		this->direction = sge::math::vec4(diffuse, 1.0f);
		this->direction = sge::math::vec4(specular, 1.0f);

		lightType = dirLight;
	}

	sge::math::vec4 LightComponent::getDirection()
	{
		return direction;
	}
	sge::math::vec4 LightComponent::getDiffuse()
	{
		return diffuse;
	}
	sge::math::vec4 LightComponent::getAmbient()
	{
		return ambient;
	}
	sge::math::vec4 LightComponent::getSpecular()
	{
		return specular;
	}

}