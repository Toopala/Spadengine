#include "Game/SpriteRenderingSystem.h"
#include "Game/SpriteComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"

#include "Renderer/GraphicsDevice.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Shader.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"

namespace sge
{
    SpriteRenderingSystem::SpriteRenderingSystem(Renderer* renderer, const std::vector<char>& vShaderData, const std::vector<char>& pShaderData) :
        renderer(renderer)
    {
        sge::VertexLayoutDescription vertexLayoutDescription = { 1,
        {
            { 0, 3, sge::VertexSemantic::POSITION }
        } };

        float vertexData[] = {
            0.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
        };

        vertexShader = renderer->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
        pixelShader = renderer->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

        pipeline = renderer->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
        vertexBuffer = renderer->getDevice()->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));
        uniformBuffer = renderer->getDevice()->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(UniformData));

        renderer->getDevice()->bindPipeline(pipeline);
        renderer->getDevice()->bindVertexBuffer(vertexBuffer);
        renderer->getDevice()->copyData(vertexBuffer, sizeof(vertexData), vertexData);
        renderer->getDevice()->debindPipeline(pipeline);
    }

    SpriteRenderingSystem::~SpriteRenderingSystem()
    {
        renderer->getDevice()->deleteShader(vertexShader);
        renderer->getDevice()->deleteShader(pixelShader);
        renderer->getDevice()->deleteBuffer(vertexBuffer);
        renderer->getDevice()->deleteBuffer(uniformBuffer);
        renderer->getDevice()->deletePipeline(pipeline);
    }

    void SpriteRenderingSystem::renderSprite(SpriteComponent* sprite)
    {
        renderer->getDevice()->bindPipeline(pipeline);
        
        uniformData.MVP = *VP * sprite->getParent()->getComponent<TransformComponent>()->getMatrix();
        uniformData.color = sprite->getColor();

        renderer->getDevice()->bindVertexUniformBuffer(uniformBuffer, 0);
        renderer->getDevice()->copyData(uniformBuffer, sizeof(uniformData), &uniformData);

        renderer->getDevice()->draw(3);

        renderer->getDevice()->debindPipeline(pipeline);
    }

    void SpriteRenderingSystem::setVP(const math::mat4& VP)
    {
        this->VP = &VP;
    }

	void SpriteRenderingSystem::addComponent(Component* component)
	{
		SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(component);

		if (spriteComponent)
		{
			components.push_back(spriteComponent);
		}
	}

    void SpriteRenderingSystem::update()
    {
        for (auto sprite : components)
        {
            renderer->pushCommand(sprite->getKey(), std::bind(&sge::SpriteComponent::render, sprite, std::placeholders::_1));
        }
    }
}