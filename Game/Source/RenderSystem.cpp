#include "Renderer/Buffer.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/RenderData.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Window.h"

#include "Resources/ResourceManager.h"
#include "Resources/ShaderResource.h"

#include "Game/RenderSystem.h"
#include "Game/Entity.h"

#include "Game/CameraComponent.h"

#include "Game/ModelComponent.h"
#include "Game/RenderComponent.h"
#include "Game/SpriteComponent.h"
#include "Game/TextComponent.h"
#include "Game/TransformComponent.h"

#include "Renderer/CubeMap.h"


namespace sge
{
    RenderSystem::RenderSystem(Window& window) :
		queue(1000),
        initialized(false),
        acceptingCommands(false),
        clearColor(0.5f, 0.6f, 0.2f, 1.0f)
	{
		device = new GraphicsDevice(window);
	}

    RenderSystem::~RenderSystem()
	{
		delete device;
	}

    void RenderSystem::init()
	{
		device->init();
        
        initShaders();

        initSpriteRendering();
        initTextRendering();
        initModelRendering();

        device->clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

        initialized = true;
	}

    void RenderSystem::deinit()
	{
        device->deleteShader(sprVertexShader);
        device->deleteShader(sprPixelShader);
        device->deleteBuffer(sprVertexBuffer);
        device->deleteBuffer(sprVertexUniformBuffer);
        device->deleteBuffer(sprPixelUniformBuffer);
        device->deletePipeline(sprPipeline);
        device->deleteBuffer(modelVertexUniformBuffer);
        device->deleteBuffer(modelPixelUniformBuffer);

		device->deinit();

        initialized = false;
	}

    void RenderSystem::renderSprites(size_t count, Entity* sprites[])
    {
        SGE_ASSERT(acceptingCommands);

        for (size_t i = 0; i < count; i++)
        {
            SpriteComponent* sprite = sprites[i]->getComponent <SpriteComponent>();

            SGE_ASSERT(sprite);

            sprite->setRenderer(this);

            for (auto camera : cameras)
            {
                uint32 distance = static_cast<uint32>(math::dot(sprite->transform->getPosition(),
                    camera->getComponent<TransformComponent>()->getPosition() +
                    camera->getComponent<TransformComponent>()->getFront()));

                if (sprite->getColor().a < 1.0f)
                    sprite->key.fields.depth = UINT32_MAX - distance;
                else
                    sprite->key.fields.depth = distance;

                queue.push(sprite->key, std::bind(&SpriteComponent::render, sprite, std::placeholders::_1));
            }
        }
    }

    void RenderSystem::renderTexts(size_t count, Entity* texts[])
    {
        SGE_ASSERT(acceptingCommands);

        for (size_t i = 0; i < count; i++)
        {
            TextComponent* text = texts[i]->getComponent <TextComponent>();

            SGE_ASSERT(text);

            text->setRenderer(this);

            for (auto camera : cameras)
            {
                uint32 distance = static_cast<uint32>(math::dot(text->transform->getPosition(),
                    camera->getComponent<TransformComponent>()->getPosition() +
                    camera->getComponent<TransformComponent>()->getFront()));

                if (text->getColor().a < 1.0f)
                    text->key.fields.depth = UINT32_MAX - distance;
                else
                    text->key.fields.depth = distance;

                queue.push(text->key, std::bind(&TextComponent::render, text, std::placeholders::_1));
            }
        }
    }

    void RenderSystem::renderModels(size_t count, Entity* models[])
    {
        SGE_ASSERT(acceptingCommands);

        for (size_t i = 0; i < count; i++)
        {
            ModelComponent* model = models[i]->getComponent <ModelComponent>();

            SGE_ASSERT(model);

            model->setRenderer(this);

            for (auto camera : cameras)
            {
                //uint32 distance = static_cast<uint32>(math::dot(model->transform->getPosition(),
                //    camera->getComponent<TransformComponent>()->getPosition() +
                //    camera->getComponent<TransformComponent>()->getFront()));

                //model->key.fields.depth = distance;

                queue.push(model->key, std::bind(&ModelComponent::render, model, std::placeholders::_1));
            }
        }
    }

    void RenderSystem::renderLights(size_t count, Entity* lights[])
    {
        SGE_ASSERT(acceptingCommands);

        for (size_t i = 0; i < count; i++)
        {
            DirLightComponent* dirLight = lights[i]->getComponent<DirLightComponent>();

            if (dirLight)
            { 
                this->dirLights.push_back(dirLight);
            }
             
            SpotLightComponent* spotLight = lights[i]->getComponent<SpotLightComponent>();

            if (spotLight)
            {
                this->spotLights.push_back(spotLight);
            }

            PointLightComponent* pointLight = lights[i]->getComponent<PointLightComponent>();

            if (pointLight)
            {
                this->pointLights.push_back(pointLight);
            }
        }
    }

    void RenderSystem::setRenderTarget(RenderTarget* renderTarget)
    {
        SGE_ASSERT(!acceptingCommands);

        device->bindRenderTarget(renderTarget);
    }

    void RenderSystem::addCameras(size_t count, Entity** cameras)
    {
        SGE_ASSERT(!acceptingCommands);
        
        for (size_t i = 0; i < count; i++)
        {
            CameraComponent* camera = cameras[i]->getComponent<CameraComponent>();

            SGE_ASSERT(camera);

            this->cameras.push_back(camera);
        }
    }

    void RenderSystem::begin()
    {
        SGE_ASSERT(initialized && !acceptingCommands);

        queue.begin();

        acceptingCommands = true;
    }

    void RenderSystem::end()
	{
        SGE_ASSERT(acceptingCommands);

        calculateLightData();

		queue.end();

        acceptingCommands = false;
	}

    void RenderSystem::render()
    {
        SGE_ASSERT(initialized && !acceptingCommands);

        for (auto& command : queue.getQueue())
        {
            command.second(device);
        }
    }

    void RenderSystem::present()
    {
        SGE_ASSERT(initialized && !acceptingCommands);

        device->swap();
    }

    void RenderSystem::clear(int flags)
    {
        SGE_ASSERT(initialized && !acceptingCommands);

        if (flags & RENDERTARGET)
        {
            device->debindRenderTarget();
        }

        if (flags & QUEUE)
        {
            queue.clear();
        }

        if (flags & COLOR || flags & DEPTH || flags & STENCIL)
        {
            device->clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        }

        if (flags & LIGHTS)
        {
            pointLights.clear();
            spotLights.clear();
            dirLights.clear();
        }

        if (flags & CAMERAS)
        {
            cameras.clear();
        }
    }

    void RenderSystem::renderSprite(SpriteComponent* sprite)
    {
        static size_t pass = 0;

        SGE_ASSERT(cameras.size() > pass);

        device->bindPipeline(sprPipeline);

        sge::Texture* texture = sprite->getTexture();

        if (texture)
        {
            device->bindTexture(texture, 0);
        }

        device->bindViewport(cameras[pass]->getViewport());

        sprVertexUniformData.MVP = cameras[pass]->getViewProj() * sprite->getComponent<TransformComponent>()->getMatrix();
        sprPixelUniformData.color = sprite->getColor();

        device->bindVertexUniformBuffer(sprVertexUniformBuffer, 0);
        device->copyData(sprVertexUniformBuffer, sizeof(sprVertexUniformData), &sprVertexUniformData);

        device->bindPixelUniformBuffer(sprPixelUniformBuffer, 1);
        device->copyData(sprPixelUniformBuffer, sizeof(sprPixelUniformData), &sprPixelUniformData);

        device->draw(6);

        if (texture)
        {
            device->debindTexture(texture, 0);
        }

        device->debindPipeline(sprPipeline);

        if (++pass >= cameras.size())
            pass = 0;
    }

    void RenderSystem::renderText(TextComponent* text)
    {
        device->bindPipeline(textPipeline);

        sge::Font* font = text->getFont();
        FT_GlyphSlot slot = font->face->glyph;

        // Updates textures if text has changed since previous rendering
        if (text->getText() != previousText)
        {
            while (charTextures.size() != 0)
            {
                charTextures.erase(charTextures.begin());
                characters.erase(characters.begin());
            }

			// Goes through all characters, loads them and stores the glyph info and textures needed to render text for later use
			// in order to make the actual drawing faster and more efficient.
            for (size_t i = 0; i < text->getText().size(); i++)
            {
                FT_Load_Char(font->face, text->getText()[i], FT_LOAD_RENDER);

                sge::Texture* texture = device->createTextTexture(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer);

                Character character;
                character.size = sge::math::vec2(slot->bitmap.width, slot->bitmap.rows);
                character.horiBearing = sge::math::vec2(slot->metrics.horiBearingX, slot->metrics.horiBearingY);
                character.vertBearing = sge::math::vec2(slot->metrics.vertBearingX, slot->metrics.vertBearingY);
                character.metrics = sge::math::vec2(slot->metrics.width, slot->metrics.height);
				character.advance = sge::math::vec2(slot->advance.x / 32, slot->advance.y / 32);
                characters.push_back(character);

                charTextures.push_back(texture);
            }
            previousText = text->getText();
        }

        // TODO added support for multiple cameras.
        static size_t pass = 0;

        SGE_ASSERT(cameras.size() > pass);

        // Render text
        sge::math::vec2 pen = { 0, 0 }; // The position where the character is drawn.
        sge::math::vec3 originalPosition = text->getParent()->getComponent<TransformComponent>()->getPosition();
        sge::math::vec3 originalScale = text->getParent()->getComponent<TransformComponent>()->getScale();
        for (size_t i = 0; i < text->getText().size(); i++)
        {
            sge::Texture* texture = charTextures[i];

            if (texture)
            {
                device->bindTexture(texture, 0);
            }

			// Calculates the y position of current character.
            pen.y = characters[i].vertBearing.y / 32 - font->characterSize;
            if (characters[i].metrics.y / 64 - characters[i].horiBearing.y / 64 > 0)
            {
                pen.y += characters[i].metrics.y / 64 - characters[i].horiBearing.y / 64;
            }

			// Places the character in its desired position.
            text->getComponent<TransformComponent>()->setPosition(originalPosition + glm::vec3(pen.x, pen.y, 0));
            text->getComponent<TransformComponent>()->setScale(originalScale * sge::math::vec3(characters[i].size.x, characters[i].size.y, 1));

            device->bindViewport(cameras[pass]->getViewport());

            sprVertexUniformData.MVP = cameras[pass]->getViewProj() * text->getComponent<TransformComponent>()->getMatrix();
			sge::math::mat4 testi = text->getComponent<TransformComponent>()->getMatrix();
            sprPixelUniformData.color = text->getColor();

			// Calculates the x position of the next character.
			pen.x += originalScale.x * characters[i].advance.x;

            device->bindVertexUniformBuffer(sprVertexUniformBuffer, 0);
            device->copyData(sprVertexUniformBuffer, sizeof(sprVertexUniformData), &sprVertexUniformData);
            device->bindPixelUniformBuffer(sprPixelUniformBuffer, 1);
            device->copyData(sprPixelUniformBuffer, sizeof(sprPixelUniformData), &sprPixelUniformData);

            device->draw(6);

            if (texture)
            {
                device->debindTexture(texture, 0);
            }
        }

        text->getComponent<TransformComponent>()->setPosition(originalPosition);
        text->getComponent<TransformComponent>()->setScale(originalScale);
        device->debindPipeline(textPipeline);

        if (++pass >= cameras.size())
            pass = 0;
    }

    void RenderSystem::renderModel(ModelComponent* model)
    {
        static size_t pass = 0;

        SGE_ASSERT(cameras.size() > pass);

        device->bindViewport(cameras[pass]->getViewport());

        modelVertexUniformData.M = model->getComponent<TransformComponent>()->getMatrix();
        modelVertexUniformData.PV = cameras[pass]->getViewProj();
		modelVertexUniformData.shininess = model->getComponent<ModelComponent>()->getShininess();
        modelPixelUniformData.CamPos = math::vec4(cameras[pass]->getComponent<TransformComponent>()->getPosition(), 1.0f);

        device->bindPipeline(model->getPipeline());

		for (size_t i = 0; i < model->getModelResource()->getMeshes().size(); i++)
		{
			device->bindIndexBuffer(model->getModelResource()->getMeshes()[i]->getIndexBuffer());
			device->bindVertexBuffer(model->getModelResource()->getMeshes()[i]->getVertexBuffer());

			device->bindVertexUniformBuffer(modelVertexUniformBuffer, 0);
			device->copyData(modelVertexUniformBuffer, sizeof(modelVertexUniformData), &modelVertexUniformData);

			
			modelPixelUniformData.hasDiffuseTex = 0;		
			modelPixelUniformData.hasNormalTex = 0;	
			modelPixelUniformData.hasSpecularTex = 0;	
			modelPixelUniformData.hasCubeTex = 0;

			Texture* diff = model->getModelResource()->getMeshes()[i]->diffuseTexture;
			Texture* norm = model->getModelResource()->getMeshes()[i]->normalTexture;
			Texture* spec = model->getModelResource()->getMeshes()[i]->specularTexture;
			CubeMap* cube = model->getCubeMap();
			if (diff)
			{
				device->bindTexture(diff, 0);
				modelPixelUniformData.hasDiffuseTex = 1;
			}

			if (norm)
			{
				device->bindTexture(norm, 1);
				modelPixelUniformData.hasNormalTex = 1;
			}

			if (spec)
			{
				device->bindTexture(spec, 2);
				modelPixelUniformData.hasSpecularTex = 1;
			}			
			
			modelPixelUniformData.glossyness = model->getGlossyness();

			if (cube)
			{
				device->bindCubeMap(cube, 3);
				modelPixelUniformData.hasCubeTex = 1;
			}			

			device->bindPixelUniformBuffer(modelPixelUniformBuffer, 1);
			device->copyData(modelPixelUniformBuffer, sizeof(modelPixelUniformData), &modelPixelUniformData);

			device->draw(model->getModelResource()->getMeshes()[i]->vertices.size());

			device->debindCubeMap(cube, 3);

			if (diff)
			{
				device->debindTexture(diff, 0);
				modelPixelUniformData.hasDiffuseTex = 0;
			}

			if (norm)
			{
				device->debindTexture(norm, 1);
				modelPixelUniformData.hasNormalTex = 0;
			}

			if (spec)
			{
				device->debindTexture(spec, 2);
				modelPixelUniformData.hasSpecularTex = 0;
			}

			if (cube)
			{
				device->debindCubeMap(cube, 3);
				modelPixelUniformData.hasCubeTex = 0;
			}
		}        

        device->debindPipeline(model->getPipeline());

        if (++pass >= cameras.size())
            pass = 0;
    }

    void RenderSystem::setClearColor(float r, float g, float b, float a)
    {
        clearColor.r = r;
        clearColor.g = g;
        clearColor.b = b;
        clearColor.a = a;
    }

    void RenderSystem::setClearColor(const math::vec4& color)
    {
        clearColor = color;
    }

    void RenderSystem::calculateLightData()
    {
        modelPixelUniformData.numofpl = (float)pointLights.size();
        modelPixelUniformData.numofdl = (float)dirLights.size();
		modelPixelUniformData.numofsl = 0.0f;
		modelPixelUniformData.pad = 0.0f;

        for (size_t i = 0; i < dirLights.size(); i++)
        {
            modelPixelUniformData.dirLights[i] = dirLights[i]->getLightData();
        }

        for (size_t i = 0; i < pointLights.size(); i++)
        {
            modelPixelUniformData.pointLights[i] = pointLights[i]->getLightData();
        }
    }

    void RenderSystem::initShaders()
    {
        Handle<ShaderResource> sprPixelShaderHandle;
        Handle<ShaderResource> sprVertexShaderHandle;
        Handle<ShaderResource> textPixelShaderHandle;


#ifdef DIRECTX11
        sprVertexShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimpleVertexShader.cso");
        sprPixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimplePixelShader.cso");
        textPixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimpleTextPixelShader.cso");
#elif OPENGL4
        sprVertexShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimpleVertexShader.glsl");
        sprPixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimplePixelShader.glsl");
        textPixelShaderHandle = ResourceManager::getMgr().load<ShaderResource>("../Assets/Shaders/SimpleTextPixelShader.glsl");
#endif

        const std::vector<char>& sprVertexShaderData = sprVertexShaderHandle.getResource<ShaderResource>()->loadShader();
        const std::vector<char>& sprPixelShaderData = sprPixelShaderHandle.getResource<ShaderResource>()->loadShader();
        const std::vector<char>& textPixelShaderData = textPixelShaderHandle.getResource<ShaderResource>()->loadShader();

        sprVertexShader = device->createShader(sge::ShaderType::VERTEX, sprVertexShaderData.data(), sprVertexShaderData.size());
        sprPixelShader = device->createShader(sge::ShaderType::PIXEL, sprPixelShaderData.data(), sprPixelShaderData.size());
        textPixelShader = device->createShader(sge::ShaderType::PIXEL, textPixelShaderData.data(), textPixelShaderData.size());
    }

    void RenderSystem::initSpriteRendering()
    {
        sge::VertexLayoutDescription vertexLayoutDescription = { 2,
        {
            { 0, 3, sge::VertexSemantic::POSITION },
            { 0, 2, sge::VertexSemantic::TEXCOORD }
        } };

        float vertexData[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        };

        sprPipeline = device->createPipeline(&vertexLayoutDescription, sprVertexShader, sprPixelShader);
        sprVertexBuffer = device->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));
        sprVertexUniformBuffer = device->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(sprVertexUniformData));
        sprPixelUniformBuffer = device->createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(sprPixelUniformData));

        device->bindPipeline(sprPipeline);
        device->bindVertexBuffer(sprVertexBuffer);
        device->copyData(sprVertexBuffer, sizeof(vertexData), vertexData);
        device->debindPipeline(sprPipeline);
    }

    void RenderSystem::initTextRendering()
    {
        sge::VertexLayoutDescription vertexLayoutDescription = { 2,
        {
            { 0, 3, sge::VertexSemantic::POSITION },
            { 0, 2, sge::VertexSemantic::TEXCOORD }
        } };

        float vertexData[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        textPipeline = device->createPipeline(&vertexLayoutDescription, sprVertexShader, textPixelShader);
        textVertexBuffer = device->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertexData));

        device->bindPipeline(textPipeline);
        device->bindVertexBuffer(textVertexBuffer);
        device->copyData(textVertexBuffer, sizeof(vertexData), vertexData);
        device->debindPipeline(textPipeline);
    }

    void RenderSystem::initModelRendering()
    {
        modelVertexUniformBuffer = device->createBuffer(BufferType::UNIFORM, BufferUsage::DYNAMIC, sizeof(modelVertexUniformData));
        modelPixelUniformBuffer = device->createBuffer(BufferType::UNIFORM, BufferUsage::DYNAMIC, sizeof(modelPixelUniformData));
    }
}