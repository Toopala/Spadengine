#include "Game/ModelRenderingSystem.h"
#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"

#include "Renderer/GraphicsDevice.h"
#include "Renderer/Renderer.h"

#include "Resources/ModelResource.h"

namespace sge
{
	ModelRenderingSystem::ModelRenderingSystem(Renderer* renderer) :
		renderer(renderer)
	{

	}

	void ModelRenderingSystem::renderModel(ModelComponent* model)
	{

	}

	void ModelRenderingSystem::setVP(const math::mat4& VP)
	{
		this->VP = &VP;
	}

	void ModelRenderingSystem::addComponent(Component* component)
	{
		ModelComponent* modelComponent = dynamic_cast<ModelComponent*>(component);

		if (modelComponent)
		{
			components.push_back(modelComponent);
		}
	}

	void ModelRenderingSystem::update()
	{
		for (auto model : components)
		{
			model->update();
			renderer->pushCommand(model->key, std::bind(&sge::ModelComponent::render, model, std::placeholders::_1));
		}
	}
}