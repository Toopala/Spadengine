#pragma once

#include "Game/System.h"
#include "Core/Math.h"
#include <vector>

namespace sge
{
	class Renderer;
	class ModelComponent;

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
	};
}