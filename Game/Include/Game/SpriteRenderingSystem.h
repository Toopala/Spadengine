#pragma once

#include "Game/System.h"
#include "Core/Math.h"
#include <vector>

namespace sge
{
    class Renderer;
    class SpriteComponent;
    struct Pipeline;
    struct Buffer;
    struct Shader;

	class SpriteRenderingSystem : public System
	{
	public:
        SpriteRenderingSystem(Renderer* renderer);
        ~SpriteRenderingSystem();

        void renderSprite(SpriteComponent* sprite);

        void setVP(const math::mat4& vp)
        {
            viewProj = vp;
        }

		void update();
		void addComponent(Component* component);

	private:
		std::vector<SpriteComponent*> components;

        Renderer* renderer;

        Pipeline* pipeline;
        Buffer* vertexBuffer;
        Buffer* vertexUniformBuffer;
        Buffer* pixelUniformBuffer;
        Shader* vertexShader;
        Shader* pixelShader;

        math::mat4 viewProj;

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