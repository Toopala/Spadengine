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
		uniformBuffer = renderer->getDevice()->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData));
		uniformBuffer2 = renderer->getDevice()->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData2));
	}

	void ModelRenderingSystem::renderModel(ModelComponent* model)
	{
		uniformData.M = model->getParent()->getComponent<TransformComponent>()->getMatrix();

		renderer->getDevice()->bindPipeline(model->getPipeline());

		//renderer->getDevice()->bindIndexBuffer(model->getModelResource()->getIndexBuffer());
		renderer->getDevice()->bindVertexBuffer(model->getModelResource()->getVertexBuffer());
		renderer->getDevice()->bindVertexUniformBuffer(uniformBuffer, 0);
		//renderer->getDevice()->bindPixelUniformBuffer(uniformBuffer2, 1);

		renderer->getDevice()->bindTexture(model->diffTexture, 0);
		renderer->getDevice()->bindTexture(model->normTexture, 1);

		renderer->getDevice()->copyData(uniformBuffer, sizeof(uniformData), &uniformData);
		//renderer->getDevice()->copyData(uniformBuffer2, sizeof(uniformData2), &uniformData2);

		renderer->getDevice()->draw(model->getModelResource()->getVerticeArray()->size());
		renderer->getDevice()->debindPipeline(model->getPipeline());
	}

	void ModelRenderingSystem::setVP(const math::mat4& VP)
	{
		uniformData.PV = VP;
	}

	void ModelRenderingSystem::setCamPos (const math::vec3& POS)
	{
		uniformData2.CamPos = POS;
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