#include "Renderer/Buffer.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderData.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Window.h"

#include "Resources/ResourceManager.h"
#include "Resources/ShaderResource.h"

#include "Game/RenderingSystem.h"

namespace sge
{
    RenderingSystem::RenderingSystem(Window& window) :
		queue(queueSize)
	{
		device = new GraphicsDevice(window);

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

        sprVertexShader = device->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
        sprPixelShader = device->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

        sprPipeline = device->createPipeline(&vertexLayoutDescription, sprVertexShader, sprPixelShader);
        sprVertexBuffer = device->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));
        sprVertexUniformBuffer = device->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(sprVertexUniformData));
        sprPixelUniformBuffer = device->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(sprPixelUniformData));

        device->bindPipeline(sprPipeline);
        device->bindVertexBuffer(sprVertexBuffer);
        device->copyData(sprVertexBuffer, sizeof(vertexData), vertexData);
        device->debindPipeline(sprPipeline);
	}

    RenderingSystem::~RenderingSystem()
	{
        device->deleteShader(sprVertexShader);
        device->deleteShader(sprPixelShader);
        device->deleteBuffer(sprVertexBuffer);
        device->deleteBuffer(sprVertexUniformBuffer);
        device->deleteBuffer(sprPixelUniformBuffer);
        device->deletePipeline(sprPipeline);

		delete device;
	}

    void RenderingSystem::init()
	{
		device->init();
	}

    void RenderingSystem::deinit()
	{
		device->deinit();
	}

    void RenderingSystem::begin()
	{
		queue.begin();
	}

    void RenderingSystem::renderSprite(SpriteComponent* sprite)
    {

    }

    void RenderingSystem::end()
	{
		queue.end();

		device->clear(0.5f, 0.6f, 0.2f, 1.0f);

		for (auto& command : queue.getQueue())
		{
			command.second(device);
		}

		queue.clear();

		device->swap();
	}
}