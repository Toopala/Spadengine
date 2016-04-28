#pragma once

#include "Game/System.h"
#include "Core/Math.h"
#include <vector>

namespace sge
{
	class Renderer;
	class ModelComponent;
	struct Buffer;

	class ModelRenderingSystem : public System
	{
	public:
		ModelRenderingSystem(Renderer* renderer);

		void renderModel(ModelComponent* model);

		void setVP(const math::mat4& VP);
		void setCamPos(const math::vec3& POS);

		void update();
		void addComponent(Component* component);
		
	private:
		std::vector<ModelComponent*> components;

		Renderer* renderer;
		
		const math::mat4* VP;

		Buffer* uniformBuffer;
		Buffer* uniformBuffer2;

		struct UniformDataComponent
		{
			sge::math::mat4 PV;
			sge::math::mat4 M;
		} uniformData;

		struct DirLight
		{
			sge::math::vec3 direction;

			sge::math::vec3 ambient;
			sge::math::vec3 diffuse;
			sge::math::vec3 specular;
		};

		struct PointLight
		{
			sge::math::vec3 position;

			float constant;
			float mylinear;
			float quadratic;

			sge::math::vec3 ambient;
			sge::math::vec3 diffuse;
			sge::math::vec3 specular;
		};

		struct UniformDataComponent2
		{
			int numberOfLights;
			DirLight dirLight;
			PointLight pointLights[40];
			sge::math::vec3 CamPos;
		} uniformData2;
	};
}