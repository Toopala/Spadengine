#pragma once

#include "Game/System.h"
#include "Core/Math.h"
#include <vector>

namespace sge
{
	class Renderer;
	class TextComponent;
	struct Pipeline;
	struct Buffer;
	struct Shader;
	struct Texture;

	class TextRenderingSystem : public System
	{
	public:
		TextRenderingSystem(Renderer* renderer);
		~TextRenderingSystem();

		void renderText(TextComponent* text);

		void setVP(const math::mat4& VP);

		void update();
		void addComponent(Component* component);

	private:
		std::vector<TextComponent*> components;
		std::vector<sge::Texture*> charTextures;

		std::string previousText = "";

		Renderer* renderer;

		Pipeline* pipeline;
		Buffer* vertexBuffer;
		Buffer* uniformBuffer;
		Shader* vertexShader;
		Shader* pixelShader;

		const math::mat4* VP;

		struct UniformData
		{
			math::mat4 MVP;
			math::vec4 color;
		} uniformData;
	};
}