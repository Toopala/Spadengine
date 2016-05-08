#pragma once

#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;

	enum LightType
	{
		pointLight = 0,
		dirLight = 1
	};

	class LightComponent : public Component
	{
	public:
		LightComponent(Entity* entity);
		~LightComponent();

		void setAsPointLight();
		void setAsDirLight(sge::math::vec3 direction, sge::math::vec3 ambient, sge::math::vec3 diffuse, sge::math::vec3 specular);

		void update();

		sge::math::vec4 getDirection();
		sge::math::vec4 getAmbient();
		sge::math::vec4 getDiffuse();
		sge::math::vec4 getSpecular();

	private:
		TransformComponent* transform;

		sge::math::vec4 direction;
		sge::math::vec4 ambient;
		sge::math::vec4 diffuse;
		sge::math::vec4 specular;

		LightType lightType;
	};
}