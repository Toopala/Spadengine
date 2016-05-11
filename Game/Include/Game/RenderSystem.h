#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include "Core/Math.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderQueue.h"

#include "Game/LightComponent.h"


namespace sge
{
	class Window;
    class RenderComponent;
    class SpriteComponent;
    class ModelComponent;
    class TextComponent;
    class CameraComponent;
    class Entity;
    struct Pipeline;
    struct Buffer;
    struct Shader;

    struct Character
    {
        sge::math::vec2 size;
        sge::math::vec2 horiBearing;
        sge::math::vec2 vertBearing;
        sge::math::vec2 metrics;
    };

    enum Clear
    {
        QUEUE           = 0x01,
        COLOR           = 0x02,
        DEPTH           = 0x04,
        STENCIL         = 0x08,
        LIGHTS          = 0x10,
        CAMERAS         = 0x20,
        RENDERTARGET    = 0x40,
        ALL             = QUEUE | COLOR | DEPTH | STENCIL | LIGHTS | CAMERAS | RENDERTARGET
    };

	class RenderSystem
	{
	public:
        RenderSystem(Window& window);
        ~RenderSystem();

		void init();
		void deinit();

		GraphicsDevice* getDevice() const { return device; }

        // TODO should we take in entities or components? 
        void renderSprites(size_t count, Entity** sprites);
        void renderTexts(size_t count, Entity** texts);
        void renderModels(size_t count, Entity** models);
        void renderLights(size_t count, Entity** lights);

        void addCameras(size_t count, Entity** cameras);
        void setRenderTarget(RenderTarget* renderTarget);

		void begin();
		void end();
        void render();
        void present();
        void clear(int flags = ALL);

        // TODO one should not directly use these methods!
        void renderSprite(SpriteComponent* sprite);
        void renderText(TextComponent* text);
        void renderModel(ModelComponent* model);

        void setClearColor(float r, float g, float b, float a);
        void setClearColor(const math::vec4& color);

	private:
		
		RenderQueue queue;
        GraphicsDevice* device;
        math::vec4 clearColor;

        // Sprite rendering data.
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

        // Model rendering data.
        Buffer* modelVertexUniformBuffer;
        Buffer* modelPixelUniformBuffer;

      

        struct ModelVertexUniformData
        {
            sge::math::mat4 PV;
            sge::math::mat4 M;
			float shininess;
        } modelVertexUniformData;

        struct ModelPixelUniformData
        {
            DirLight dirLight;
            PointLight pointLights[40];
            sge::math::vec4 CamPos;
            int numofpl;
            int numofdl;
            int pad[2];
        } modelPixelUniformData;

        // Text rendering data.
        std::vector<sge::Texture*> charTextures; // TODO who deletes these?
        std::vector<Character> characters;
        std::string previousText = "";

        // Global rendering data.
        std::vector<CameraComponent*> cameras;
        std::vector<LightComponent*> lights;

        bool initialized;
        bool acceptingCommands;
	};
}
