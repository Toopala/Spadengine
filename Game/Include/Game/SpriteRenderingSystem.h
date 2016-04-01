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

        void setVP(const math::mat4& VP);

		void update();
		void addComponent(Component* component);

	private:
		std::vector<SpriteComponent*> components;

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