#pragma once

#include "Game/System.h"
#include "Core/Math.h"
#include <vector>

namespace sge
{
	class RenderSystem;
	class TextComponent;
	struct Pipeline;
	struct Buffer;
	struct Shader;
	struct Texture;

	class TextRenderingSystem : public System
	{
	public:
        TextRenderingSystem(RenderSystem* renderer);
		~TextRenderingSystem();

		void renderText(TextComponent* text);

		void setVP(const math::mat4& VP);

		void update();
		void addComponent(Component* component);

	private:
		std::vector<TextComponent*> components;
		std::vector<sge::Texture*> charTextures;

		std::string previousText = "";

		RenderSystem* renderer;

		Pipeline* pipeline;
		Buffer* vertexBuffer;
		Buffer* uniformBuffer;
		Buffer* vertexUniformBuffer;
		Buffer* pixelUniformBuffer;
		Shader* vertexShader;
		Shader* pixelShader;

		const math::mat4* VP;

		struct VertexUniformData
		{
			math::mat4 MVP;
		} vertexUniformData;

		struct PixelUniformData
		{
			math::vec4 color;
		} pixelUniformData;
	};
}