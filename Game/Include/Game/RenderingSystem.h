#pragma once

#include <cstddef>

#include "Core/Math.h"
#include "Renderer/RenderQueue.h"
#include "Renderer/GraphicsDevice.h"

namespace sge
{
	class GraphicsDevice;
	class Window;
    class SpriteComponent;
    struct Pipeline;
    struct Buffer;
    struct Shader;

	class RenderingSystem
	{
	public:
        RenderingSystem(Window& window);
        ~RenderingSystem();

		void init();
		void deinit();

		GraphicsDevice* getDevice() const { return device; }

        void renderSprite(SpriteComponent* sprite);

		inline void pushCommand(const RenderCommand command, const RenderQueue::RenderFunction renderFunction)
		{
			queue.push(command, renderFunction);
		}

		void begin();
		void end();

	private:
		GraphicsDevice* device;
		RenderQueue queue;

		const size_t queueSize = 1000;

        Pipeline* sprPipeline;
        Buffer* sprVertexBuffer;
        Buffer* sprVertexUniformBuffer;
        Buffer* sprPixelUniformBuffer;
        Shader* sprVertexShader;
        Shader* sprPixelShader;

        struct SprVertexUniformData
        {
            math::mat4 MVP;
        } sprVertexUniformData;

        struct SprPixelUniformData
        {
            math::vec4 color;
        } sprPixelUniformData;
	};
}
