#include "Game/TextRenderingSystem.h"
#include "Game/TextComponent.h"
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
	TextRenderingSystem::TextRenderingSystem(Renderer* renderer) :
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

	TextRenderingSystem::~TextRenderingSystem()
	{
		renderer->getDevice()->deleteShader(vertexShader);
		renderer->getDevice()->deleteShader(pixelShader);
		renderer->getDevice()->deleteBuffer(vertexBuffer);
		renderer->getDevice()->deleteBuffer(vertexUniformBuffer);
		renderer->getDevice()->deleteBuffer(pixelUniformBuffer);
		renderer->getDevice()->deletePipeline(pipeline);
	}

	void TextRenderingSystem::renderText(TextComponent* text)
	{
		renderer->getDevice()->bindPipeline(pipeline);

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

			for (int i = 0; i < text->getText().size(); i++)
			{
				FT_Load_Char(font->face, text->getText()[i], FT_LOAD_RENDER);

				unsigned char* expandedData = new unsigned char[2 * slot->bitmap.width * slot->bitmap.rows];
				for (int j = 0; j < slot->bitmap.rows; j++)
				{
					for (int k = 0; k < slot->bitmap.width; k++)
					{
						expandedData[2 * (k + j * slot->bitmap.width)] = 255;
						expandedData[2 * (k + j * slot->bitmap.width) + 1] = (k >= slot->bitmap.width || j >= slot->bitmap.rows) ? 0 : slot->bitmap.buffer[k + slot->bitmap.width * j];
					}
				}

				sge::Texture* texture = renderer->getDevice()->createTextTexture(slot->bitmap.width, slot->bitmap.rows, expandedData);
				Character character;
				character.size = sge::math::vec2(slot->bitmap.width, slot->bitmap.rows);
				character.horiBearing = sge::math::vec2(slot->metrics.horiBearingX, slot->metrics.horiBearingY);
				character.vertBearing = sge::math::vec2(slot->metrics.vertBearingX, slot->metrics.vertBearingY);
				character.metrics = sge::math::vec2(slot->metrics.width, slot->metrics.height);
				characters.push_back(character);
				charTextures.push_back(texture);
				delete[] expandedData;
			}
			previousText = text->getText();
		}
		
		// Render text
		sge::math::vec2 pen = { 0, 0 };
		sge::math::vec3 originalPosition = text->getParent()->getComponent<TransformComponent>()->getPosition();
		sge::math::vec3 originalScale = text->getParent()->getComponent<TransformComponent>()->getScale();
		for (int i = 0; i < text->getText().size(); i++)
		{
			sge::Texture* texture = charTextures[i];

			if (texture)
			{
				renderer->getDevice()->bindTexture(texture, 0);
			}

			pen.y = characters[i].vertBearing.y / 32 - font->characterSize;
			if (characters[i].metrics.y / 64 - characters[i].horiBearing.y / 64 > 0)
			{
				pen.y += characters[i].metrics.y / 64 - characters[i].horiBearing.y / 64;
			}

			text->getParent()->getComponent<TransformComponent>()->setPosition(originalPosition + glm::vec3(pen.x, pen.y, 0));
			text->getParent()->getComponent<TransformComponent>()->setScale(originalScale * sge::math::vec3(characters[i].size.x, characters[i].size.y, 1));

			vertexUniformData.MVP = *VP * text->getParent()->getComponent<TransformComponent>()->getMatrix();
			pixelUniformData.color = text->getColor();

			pen.x += originalScale.x * characters[i].size.x * 2;

			renderer->getDevice()->bindVertexUniformBuffer(vertexUniformBuffer, 0);
			renderer->getDevice()->copyData(vertexUniformBuffer, sizeof(vertexUniformData), &vertexUniformData);
			renderer->getDevice()->bindPixelUniformBuffer(pixelUniformBuffer, 1);
			renderer->getDevice()->copyData(pixelUniformBuffer, sizeof(pixelUniformData), &pixelUniformData);

			renderer->getDevice()->draw(6);

			if (texture)
			{
				renderer->getDevice()->debindTexture(texture, 0);
			}
		}

		text->getParent()->getComponent<TransformComponent>()->setPosition(originalPosition);
		text->getParent()->getComponent<TransformComponent>()->setScale(originalScale);
		renderer->getDevice()->debindPipeline(pipeline);
	}

	void TextRenderingSystem::setVP(const math::mat4& VP)
	{
		this->VP = &VP;
	}

	void TextRenderingSystem::addComponent(Component* component)
	{
		TextComponent* textComponent = dynamic_cast<TextComponent*>(component);

		if (textComponent)
		{
			components.push_back(textComponent);
		}
	}

	void TextRenderingSystem::update()
	{
		for (auto text : components)
		{
			text->update();
			renderer->pushCommand(text->key, std::bind(&sge::TextComponent::render, text, std::placeholders::_1));
		}
	}
}