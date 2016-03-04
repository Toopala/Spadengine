#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Resources/Resource.h"
#include "Core/Math.h"

namespace sge
{
	class TextResource : public sge::Resource
	{
	public:
		TextResource(const std::string& resourcePath);
		~TextResource();

		// Setters:
		void setFont(const std::string& resourcePath);
		void setText(std::string message);
		void setCharacterSize(float size);
		void setColor(sge::math::vec4 c);
		void setPosition(sge::math::vec3 p);
		void setScale(sge::math::vec2 s);
		void setRotation(float a);
		void move(sge::math::vec3 p);
		void scale(sge::math::vec2 s);
		void rotate(float a);

		// Getters:
		std::string getText();
		float getCharacterSize();
		sge::math::vec4 getColor();
		sge::math::vec3 getPosition();
		sge::math::vec2 getScale();
		float getRotation();
		sge::math::vec2 getBounds();
		sge::math::vec2 getGlobalBounds();

	private:
		FT_Library library;
		FT_Face face;
		FT_Error error;

		float characterSize = 12;
		std::string text = "";
		sge::math::vec4 textColor = { 1.0, 1.0, 1.0, 1.0 };
		sge::math::vec3 textPosition = { 0.0, 0.0, 0.0 };
		sge::math::vec2 textScale = { 1.0, 1.0 };
		float textAngle = 0;
	};
}