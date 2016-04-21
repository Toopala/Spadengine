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

        sge::VertexLayoutDescription vertexLayoutDescription = { 2,
        {
            { 0, 3, sge::VertexSemantic::POSITION },
            { 0, 2, sge::VertexSemantic::TEXCOORD }
        } };

        float vertexData[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        const std::vector<char>& vShaderData = vertexShaderHandle.getResource<ShaderResource>()->loadShader();
        const std::vector<char>& pShaderData = pixelShaderHandle.getResource<ShaderResource>()->loadShader();

        vertexShader = renderer->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
        pixelShader = renderer->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

        pipeline = renderer->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
        vertexBuffer = renderer->getDevice()->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));
        vertexUniformBuffer = renderer->getDevice()->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(vertexUniformData));
        pixelUniformBuffer = renderer->getDevice()->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(pixelUniformData));

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
        renderer->getDevice()->deleteBuffer(vertexUniformBuffer);
        renderer->getDevice()->deleteBuffer(pixelUniformBuffer);
        renderer->getDevice()->deletePipeline(pipeline);
    }

    void SpriteRenderingSystem::renderSprite(SpriteComponent* sprite)
    {
        renderer->getDevice()->bindPipeline(pipeline);

        sge::Texture* texture = sprite->getTexture();

        if (texture)
        {
            renderer->getDevice()->bindTexture(texture, 0);
        }
        
        vertexUniformData.MVP = *VP * sprite->getParent()->getComponent<TransformComponent>()->getMatrix();
        pixelUniformData.color = sprite->getColor();

        renderer->getDevice()->bindVertexUniformBuffer(vertexUniformBuffer, 0);
        renderer->getDevice()->copyData(vertexUniformBuffer, sizeof(vertexUniformData), &vertexUniformData);

        renderer->getDevice()->bindPixelUniformBuffer(pixelUniformBuffer, 1);
        renderer->getDevice()->copyData(pixelUniformBuffer, sizeof(pixelUniformData), &pixelUniformData);

        renderer->getDevice()->draw(6);

        if (texture)
        {
            renderer->getDevice()->debindTexture(texture, 0);
        }

        renderer->getDevice()->debindPipeline(pipeline);
    }

    void SpriteRenderingSystem::setVP(const math::mat4& VP)
    {
        this->VP = &VP;
    }

	void SpriteRenderingSystem::addComponent(Component* component)
	{
		SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(component);

        spriteComponent->setRenderingSystem(this);

		if (spriteComponent)
		{
			components.push_back(spriteComponent);
		}
	}

    void SpriteRenderingSystem::update()
    {
        for (auto sprite : components)
        {
            sprite->update();
            renderer->pushCommand(sprite->key, std::bind(&sge::SpriteComponent::render, sprite, std::placeholders::_1));
        }
    }
}