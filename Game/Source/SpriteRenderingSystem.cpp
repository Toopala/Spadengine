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

#include "Resources/ResourceManager.h"
#include "Resources/ShaderResource.h"
#include "Resources/TextureResource.h"

namespace sge
{
    SpriteRenderingSystem::SpriteRenderingSystem(Renderer* renderer) :
        renderer(renderer)
    {

        Handle<ShaderResource> pixelShaderHandle;
        Handle<ShaderResource> vertexShaderHandle;

#ifdef DIRECTX11
        vertexShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../../Shaders/Compiled/SimpleVertexShader.cso");
        pixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../../Shaders/Compiled/SimplePixelShader.cso");
#elif OPENGL4
        vertexShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../../Shaders/Compiled/SimpleVertexShader.glsl");
        pixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../../Shaders/Compiled/SimplePixelShader.glsl");
#endif


        sge::VertexLayoutDescription vertexLayoutDescription = { 1,
        {
            { 0, 3, sge::VertexSemantic::POSITION }
        } };

        float vertexData[] = {
            0.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
        };

        const std::vector<char>& vShaderData = vertexShaderHandle.getResource<ShaderResource>()->loadShader();
        const std::vector<char>& pShaderData = pixelShaderHandle.getResource<ShaderResource>()->loadShader();

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