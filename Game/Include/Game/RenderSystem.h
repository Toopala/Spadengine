#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include "Core/Math.h"
#include "Renderer/GraphicsDevice.h"

#include "Game/LightComponent.h"
#include "Game/PointLightComponent.h"
#include "Game/DirLightComponent.h"
#include "Game/SpotLightComponent.h"

namespace sge
{
    const int MAX_DIR_LIGHTS = 10;
    const int MAX_POINT_LIGHTS = 40;
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
        sge::math::vec2 advance;
    };

    enum Clear
    {
        QUEUE = 0x01,
        COLOR = 0x02,
        DEPTH = 0x04,
        STENCIL = 0x08,
        LIGHTS = 0x10,
        CAMERAS = 0x20,
        RENDERTARGET = 0x40,
        ALL = QUEUE | COLOR | DEPTH | STENCIL | LIGHTS | CAMERAS | RENDERTARGET
    };

    enum RenderMode
    {
        FORWARD,
        DEFERRED
    };

#ifdef DIRECTX11
    __declspec(align(16))
#endif
	class RenderSystem
	{
	public:
        RenderSystem(Window& window);
        ~RenderSystem();

		void init();
		void deinit();

		GraphicsDevice* getDevice() const { return device; }

        // TODO should we take in entities or components? 
        void renderSprites(size_t count, Entity* sprites[]);
        void renderTexts(size_t count, Entity* texts[]);
        void renderModels(size_t count, Entity* models[]);
        void renderLights(size_t count, Entity* lights[]);

        void addCameras(size_t count, Entity* cameras[]);
        void setRenderTarget(RenderTarget* renderTarget);

		void begin();
		void end();
        void render(RenderMode renderMode = FORWARD);
        void present();
        void clear(int flags = ALL);

        void setClearColor(float r, float g, float b, float a);
        void setClearColor(const math::vec4& color);

	private:
        void renderSprite(SpriteComponent* sprite, CameraComponent* camera);
        void renderText(TextComponent* text, CameraComponent* camera);
        void renderModel(ModelComponent* model, CameraComponent* camera);
        
        void initShaders();
        void initSpriteRendering();
        void initTextRendering();
        void initModelRendering();
        void initLights();

        void renderForward();
        void renderDeferred();

        void updateLightData();
		
        GraphicsDevice* device;
        math::vec4 clearColor;

        // Vectors for different render components.
        std::vector<SpriteComponent*> spritesToRender;
        std::vector<ModelComponent*> modelsToRender;
        std::vector<TextComponent*> textsToRender;

        // Sprite rendering data.
        Pipeline* sprPipeline;
        Buffer* sprVertexBuffer;
        Buffer* sprVertexUniformBuffer;
        Buffer* sprPixelUniformBuffer;
        Shader* sprVertexShader;
        Shader* sprPixelShader;
		
#ifdef DIRECTX11
        __declspec(align(16))
#endif
        struct SprVertexUniformData
        {
            math::mat4 MVP;
        } sprVertexUniformData;

#ifdef DIRECTX11
        __declspec(align(16))
#endif
        struct SprPixelUniformData
        {
            math::vec4 color;
        } sprPixelUniformData;

        // Text rendering data.
        Pipeline* textPipeline;
        Buffer* textVertexBuffer;
        Shader* textPixelShader;

        // Model rendering data.
        Buffer* modelVertexUniformBuffer;
        Buffer* modelPixelUniformBuffer;

#ifdef DIRECTX11
        __declspec(align(16))
#endif
        struct ModelVertexUniformData
        {
            sge::math::mat4 PV;
            sge::math::mat4 M;
			float shininess;
        } modelVertexUniformData;

        struct ModelPixelUniformData
        {
            sge::math::vec4 cameraPosition;
            int hasDiffuseTex;
            int hasNormalTex;
            int hasSpecularTex;
            int hasCubeTex;
            float glossyness;
            float pad[3]; // TODO do we even need this?
        } modelPixelUniformData;

        // Light rendering data.
        Buffer* pixelLightBuffer;

#ifdef DIRECTX11
        __declspec(align(16))
#endif
        struct PixelLightData
        {
            DirLight dirLights[MAX_DIR_LIGHTS];
            PointLight pointLights[MAX_POINT_LIGHTS];
            float numofpl;
            float numofdl;
            float numofsl;
            float pad;
        } pixelLightData;

#ifdef DIRECTX11
        __declspec(align(16))
#endif

        // Text rendering data.
        std::vector<sge::Texture*> charTextures; // TODO who deletes these?
        std::vector<Character> characters;
        std::string previousText = "";

        // Global rendering data.
        std::vector<CameraComponent*> cameras;
        std::vector<SpotLightComponent*> spotLights;
        std::vector<DirLightComponent*> dirLights;
        std::vector<PointLightComponent*> pointLights;

        bool initialized;
        bool acceptingCommands;

        enum UniformBufferSlots
        {
            VERTEX = 0,
            PIXEL = 1,
            LIGHTS = 2
        };
	};
}