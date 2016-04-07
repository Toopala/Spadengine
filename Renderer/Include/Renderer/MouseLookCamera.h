#pragma once

#include "Core/Math.h"

namespace sge
{
	class MouseLookCamera
	{
	public:
		MouseLookCamera(float FOV, float aspectRatio, float near, float far, bool enableMouse, sge::math::vec3 POS, sge::math::vec3 front, sge::math::vec3 up);
		~MouseLookCamera();

		void update(int mouseX, int mouseY);
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
		bool firstMouse = true;
		bool mouseEnabled;

		float sensitivity;
	};
}