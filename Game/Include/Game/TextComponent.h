#pragma once
#include "Game/RenderComponent.h"
#include "Core/Math.h"
#include "Resources/FontResource.h"

#include <string>

namespace sge
{
	class TransformComponent;

	class TextComponent : public RenderComponent
	{
	public:
		TextComponent(Entity* ent);
		TextComponent(Entity* ent, sge::Font* font, const sge::math::vec4& col);
		~TextComponent();

		void render(GraphicsDevice* device);
		void update();

		void setFont(sge::Font* font);
		void setColor(const math::vec4& color);
		void setText(const std::string& text);

		sge::Font* getFont();
		const math::vec4& getColor();
		const std::string& getText();

	private:
		sge::Font* font;
		math::vec4 color;
		std::string text;

		TransformComponent* transform;
	};
}
