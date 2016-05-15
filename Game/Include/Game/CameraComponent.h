#pragma once

#include "Renderer/Viewport.h"
#include "Game/Component.h"
#include "Core/Math.h"

namespace sge
{
	class TransformComponent;

	class CameraComponent : public Component
	{
	public:
		CameraComponent(Entity* ent);

        // TODO we can't have a general setup method because we need to support both ortho and perspective projections.
		void update();

        void setPerspective(float fov, float aspectRatio, float near, float far);
        void setOrtho(float left, float right, float bottom, float top, float near, float far);

        void setViewport(int x, int y, unsigned int width, unsigned int height);
        void setViewport(const Viewport& viewport);

        Viewport* getViewport()
        {
            return  &viewport;
        }

        const sge::math::mat4& getViewProj()
        {
            return viewProj;
        }

	private:
        void updateView();

        Viewport viewport;

		math::mat4 viewProj;
		math::mat4 proj;

        TransformComponent* transform;
	};
}