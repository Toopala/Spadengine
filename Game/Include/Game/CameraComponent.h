#pragma once

#include "Game/RenderingComponent.h"
#include "Core/Math.h"
#include "Spade/Spade.h"

namespace sge
{
	class TransformComponent;

	class CameraComponent : public RenderingComponent
	{
	public:
		CameraComponent(Entity* ent);
		~CameraComponent();

		void setupCamera(float FOV, float aspectRatio, float near, float far, bool enableMouse, sge::math::vec3 POS, sge::math::vec3 front, sge::math::vec3 up);
		void update();
		void render(GraphicsDevice* device); // TODO: Render some cone object
		void setRenderingSystem(); //TODO

		sge::math::mat4 getVp();

		void enableMouse();
		void disableMouse();

		void setFrontByVector(sge::math::vec3 front);
		void setPosition(sge::math::vec3 POS);
		void setSensitivity(float sensitivity);

		sge::math::vec3 getFront();
	private:
		sge::math::mat4 V;
		sge::math::mat4 P;
		sge::math::mat4 VP;

		glm::vec3 cameraFront;
		glm::vec3 cameraPos;
		glm::vec3 cameraUp;

		float lastX, lastY;
		float yaw, pitch;

		int mouseXpos, mouseYpos;
		int mousseX, mousseY;

		bool mouseEnabled;

		float sensitivity;

		TransformComponent* transform;
		// SomeObjectRenderingSystem* renderingsystem;

		int enableX, enableY;
	};
}