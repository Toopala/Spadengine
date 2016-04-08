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

		void update();
		void addComponent(Component* component);
		
	private:
		std::vector<ModelComponent*> components;

		Renderer* renderer;
		
		const math::mat4* VP;

		Buffer* uniformBuffer;

		struct UniformDataComponent
		{
			sge::math::mat4 PV;
			sge::math::mat4 M;
		} uniformData;
	};
}