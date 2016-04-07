#include "Renderer\MouseLookCamera.h"

namespace sge
{
	MouseLookCamera::MouseLookCamera(float FOV, float aspectRatio, float near, float far, bool enableMouse, sge::math::vec3 POS, sge::math::vec3 front, sge::math::vec3 up)
	{
		this->cameraPos = POS;
		this->cameraFront = front;
		this->cameraUp = up;

		P = sge::math::perspective(sge::math::radians(FOV), aspectRatio, near, far);
		V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		VP = P * V;
		mouseEnabled = enableMouse;

		firstMouse = true;

		sensitivity = 0.15f;
	}

	MouseLookCamera::~MouseLookCamera()
	{

	}

	void MouseLookCamera::update(int mouseX, int mouseY)
	{
		if (mouseEnabled)
		{
			if (firstMouse)
			{
				lastX += mouseX;
				lastY += mouseY;
				firstMouse = false;
			}

			mousseX += mouseX;
			mousseY += mouseY;

			float xoffset = mousseX - lastX;
			float yoffset = lastY - mousseY;
			lastX = static_cast<float>(mousseX);
			lastY = static_cast<float>(mousseY);

			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			sge::math::vec3 front;
			front.x = sge::math::cos(sge::math::radians(pitch)) * sge::math::cos(sge::math::radians(yaw));
			front.y = sge::math::sin(sge::math::radians(pitch));
			front.z = sge::math::cos(sge::math::radians(pitch)) * sge::math::sin(sge::math::radians(yaw));
			cameraFront = sge::math::normalize(front);
		}

		V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		VP = P * V;
	}

	sge::math::mat4 MouseLookCamera::getVp()
	{
		return VP;
	}

	void MouseLookCamera::enableMouse()
	{
		mouseEnabled = true;
	}

	void MouseLookCamera::disableMouse()
	{
		mouseEnabled = false;
	}

	void MouseLookCamera::setFrontByVector(sge::math::vec3 front)
	{
		cameraFront = front;
	}

	void MouseLookCamera::setPosition(sge::math::vec3 POS)
	{
		cameraPos = POS;
	}

	void MouseLookCamera::setSensitivity(float sensitivity)
	{
		this->sensitivity = sensitivity;
	}

	sge::math::vec3 MouseLookCamera::getFront()
	{
		return cameraFront;
	}
}
