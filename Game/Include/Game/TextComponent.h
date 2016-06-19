#pragma once
#include "Game/Component.h"
#include "Core/Math.h"
#include "Resources/FontResource.h"

#include <string>

namespace sge
{
	class TransformComponent;

    class TextComponent : public Component
	{
	public:
		TextComponent(Entity* ent);
		TextComponent(Entity* ent, sge::Font* font, const sge::math::vec4& col);
		~TextComponent();

		void update();

		void setFont(sge::Font* font);
		void setColor(const math::vec4& color);
		void setText(const std::string& text);

		sge::Font* getFont();
		const math::vec4& getColor();
		const std::string& getText();

        TransformComponent* transform;
	private:
		sge::Font* font;
		math::vec4 color;
		std::string text;
	};
}
