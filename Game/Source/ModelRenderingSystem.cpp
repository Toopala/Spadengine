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
		uniformBuffer = renderer->getDevice()->createBuffer(BufferType::UNIFORM, BufferUsage::DYNAMIC, sizeof(uniformData));
		uniformBuffer2 = renderer->getDevice()->createBuffer(BufferType::UNIFORM, BufferUsage::DYNAMIC, sizeof(uniformData2));
	
		uniformData2.numberOfLights = 1;
		uniformData2.pointLights[0].position = math::vec3(0.0, 4.0, 0.0);
		uniformData2.pointLights[0].constant = float(1.0);
		uniformData2.pointLights[0].mylinear = float(0.09);
		uniformData2.pointLights[0].quadratic = float(0.032);
		uniformData2.pointLights[0].ambient = math::vec3(0.05, 0.05, 0.05);
		uniformData2.pointLights[0].diffuse = math::vec3(0.8, 0.8, 0.8);
		uniformData2.pointLights[0].specular = math::vec3(1.0, 1.0, 1.0);

		uniformData2.dirLight.direction = math::vec3(0.0, 0.0, -1.0);
		uniformData2.dirLight.ambient = math::vec3(0.05, 0.05, 0.05);
		uniformData2.dirLight.diffuse = math::vec3(0.8, 0.8, 0.8);
		uniformData2.dirLight.specular = math::vec3(0.5, 0.5, 0.5);
	}

	void ModelRenderingSystem::renderModel(ModelComponent* model)
	{
		uniformData.M = model->getParent()->getComponent<TransformComponent>()->getMatrix();

		renderer->getDevice()->bindPipeline(model->getPipeline());

		renderer->getDevice()->bindIndexBuffer(model->getModelResource()->getIndexBuffer());
		renderer->getDevice()->bindVertexBuffer(model->getModelResource()->getVertexBuffer());

		renderer->getDevice()->bindVertexUniformBuffer(uniformBuffer, 0);
		renderer->getDevice()->copyData(uniformBuffer, sizeof(uniformData), &uniformData);
		renderer->getDevice()->bindPixelUniformBuffer(uniformBuffer2, 1);
		renderer->getDevice()->copyData(uniformBuffer2, sizeof(uniformData2), &uniformData2);

		renderer->getDevice()->bindTexture(model->diffTexture, 0);
		renderer->getDevice()->bindTexture(model->normTexture, 1);
		renderer->getDevice()->bindTexture(model->specTexture, 2);

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
			renderer->pushCommand(model->key, std::bind(&ModelComponent::render, model, std::placeholders::_1));
		}
	}
}