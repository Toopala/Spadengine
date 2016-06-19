#include "Game/TextComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/RenderSystem.h"
#include "Core/Assert.h"

namespace sge
{
	TextComponent::TextComponent(Entity* ent) :
        Component(ent),
		color(1.0f),
		font(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		// We need transform!
		SGE_ASSERT(transform);
	}

	TextComponent::TextComponent(Entity* ent, sge::Font* font, const sge::math::vec4& col) :
        Component(ent),
		color(col),
		font(font)
	{
		transform = getParent()->getComponent<TransformComponent>();

		// We need transform!
		SGE_ASSERT(transform);
	}

	TextComponent::~TextComponent()
	{
	}

	void TextComponent::update()
	{
	}

	void TextComponent::setColor(const math::vec4& color)
	{
		this->color = color;
	}

	void TextComponent::setFont(sge::Font* font)
	{
		this->font = font;
	}

	void TextComponent::setText(const std::string& text)
	{
		this->text = text;
	}

	sge::Font* TextComponent::getFont()
	{
		return font;
	}

	const math::vec4& TextComponent::getColor()
	{
		return color;
	}

	const std::string& TextComponent::getText()
	{
		return text;
	}
}
