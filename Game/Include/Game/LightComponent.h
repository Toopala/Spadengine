#pragma once

#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;

	enum LightType
	{
		pointLightEnum = 0,
		dirLightEnum = 1
	};

	///////////////////////////////
	// TODO move these to LightComponent!
	struct DirLight
	{
		sge::math::vec4 direction;
		sge::math::vec4 ambient;
		sge::math::vec4 diffuse;
		sge::math::vec4 specular;
	};

	struct PointLight
	{
		sge::math::vec4 position;
		sge::math::vec4 ambient;
		sge::math::vec4 diffuse;
		sge::math::vec4 specular;

		float constant;
		float mylinear;
		float quadratic;
		float pad;
	};
	//////////////////////////////

	class LightComponent : public Component
	{
	public:
		LightComponent(Entity* entity);
		~LightComponent();

		void setAsPointLight(PointLight* pointLight);
		void setAsDirLight(DirLight* dirLight);

		void update();

	private:
		TransformComponent* transform;

		LightType lightType;

		DirLight* dirLight;
		PointLight* pointLight;
	};
}