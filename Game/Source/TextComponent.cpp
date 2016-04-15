#include "Game/TextComponent.h"
#include "Game/TransformComponent.h"
#include "Game/Entity.h"
#include "Game/TextRenderingSystem.h"
#include "Core/Assert.h"

namespace sge
{
	TextComponent::TextComponent(Entity* ent) :
		RenderingComponent(ent),
		color(1.0f),
		font(nullptr),
		renderingSystem(nullptr)
	{
		transform = getParent()->getComponent<TransformComponent>();

		// We need transform!
		SGE_ASSERT(transform);
	}

	TextComponent::TextComponent(Entity* ent, sge::TextRenderingSystem* system, sge::Font* font, const sge::math::vec4& col) :
		RenderingComponent(ent),
		color(col),
		font(font),
		renderingSystem(system)
	{
		transform = getParent()->getComponent<TransformComponent>();

		// We need transform!
		SGE_ASSERT(transform);
	}

	TextComponent::~TextComponent()
	{
	}

	void TextComponent::render(GraphicsDevice* device)
	{
		SGE_ASSERT(renderingSystem);

		
		renderingSystem->renderText(this);
	}

	void TextComponent::update()
	{
		key.fields.translucent = (color.a < 1.0f) ? 1 : 0;
		key.fields.depth = static_cast<uint64>(transform->getPosition().z);
	}

	void TextComponent::setRenderingSystem(TextRenderingSystem* system)
	{
		renderingSystem = system;
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
