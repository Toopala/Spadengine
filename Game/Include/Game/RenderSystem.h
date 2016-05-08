#pragma once

#include <cstddef>
#include <vector>

#include "Core/Math.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderQueue.h"

namespace sge
{
	class Window;
    class RenderComponent;
    class SpriteComponent;
    class ModelComponent;
    class TextComponent;
    class LightComponent;
    class CameraComponent;
    class Entity;
    struct Pipeline;
    struct Buffer;
    struct Shader;

	class RenderSystem
	{
	public:
        RenderSystem(Window& window);
        ~RenderSystem();

		void init();
		void deinit();

		GraphicsDevice* getDevice() const { return device; }

        // TODO should we take in entities or components? 
        void renderSprites(size_t count, Entity* sprites);
        void renderTexts(size_t count, Entity* texts);
        void renderModels(size_t count, Entity* models);
        void renderLights(size_t count, Entity* lights);

        void setRenderTargets(size_t count, RenderTarget* renderTargets);
        void setCameras(size_t count, Entity* cameras);

		void begin();
		void end();
        void present();
        void clear();

        void renderSprite(SpriteComponent* sprite);
        void renderText(TextComponent* text);
        void renderModel(ModelComponent* model);

	private:
		
		RenderQueue queue;
        GraphicsDevice* device;

        Pipeline* sprPipeline;
        Buffer* sprVertexBuffer;
        Buffer* sprVertexUniformBuffer;
        Buffer* sprPixelUniformBuffer;
        Shader* sprVertexShader;
        Shader* sprPixelShader;

        std::vector<CameraComponent*> cameras;
        std::vector<LightComponent*> lights;

        struct SprVertexUniformData
        {
            math::mat4 MVP;
        } sprVertexUniformData;

        struct SprPixelUniformData
        {
            math::vec4 color;
        } sprPixelUniformData;

        bool initialized;
        bool acceptingCommands;
	};
}
