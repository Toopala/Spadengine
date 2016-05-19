#pragma once

#include "Renderer/Viewport.h"
#include "Spade/Spade.h"
#include "Game/Scene.h"
#include "Core/Math.h"
#include "Resources/ModelResource.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "Game/Entity.h"
#include "Game/EntityManager.h"
#include "Game/SystemManager.h"
#include "Game/CameraComponent.h"
#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"
#include "Game/ComponentFactory.h"
#include "Game/PhysicsComponent.h"

// FORWARD DECLARE
struct sge::Pipeline;
struct sge::Buffer;
struct sge::Viewport;
struct sge::Shader;
struct sge::Texture;

struct Vertex;

struct UniformData2
{
	sge::math::mat4 PV;
	sge::math::mat4 M;
};

class Scene : public sge::Scene
{
public:
	Scene(sge::Spade* engine);
	~Scene();

	void update(float dt);
	void draw();
	void interpolate(float alpha);

	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);
	void mouseLook();

private:
	// Engine:
	sge::Spade *engine;
	sge::RenderSystem *renderer;
	sge::EntityManager *entityManager;

	// Pipeline:
	sge::Pipeline *pipeline;

	// Buffers:
	sge::Buffer *vertexBuffer;
	sge::Buffer *uniformBuffer;

	// Shaders:
	sge::Shader *vertexShader;
	sge::Shader *pixelShader;

	// Lights:
	sge::Entity *pointLightEntity;
	sge::TransformComponent *pointLightTransform;
	sge::ModelComponent *pointLightModel;
	sge::PointLightComponent *pointLightComponent;

	sge::Entity *pointLightEntity2;
	sge::TransformComponent *pointLightTransform2;
	sge::ModelComponent *pointLightModel2;
	sge::PointLightComponent *pointLightComponent2;

	sge::Entity *dirLightEntity;
	sge::TransformComponent *dirLightTransform;
	sge::ModelComponent *dirLightModel;
	sge::DirLightComponent* dirLightComponent;

	// Cameras:
	std::vector<sge::Entity*> cameras;
	sge::TransformComponent* cameraTransform;
	sge::CameraComponent *cameraComponent;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// Mouse:
	sge::math::ivec2 mouse;
	float lastX, lastY;
	float yaw, pitch;
	int mouseXpos, mouseYpos;
	int mouseX, mouseY;
	bool firstMouse = true;
	bool useMouse = false;

	// Models:

	// Cube:
	sge::Handle<sge::ModelResource> cubeHandle;
	sge::Entity *cubeEntity;
	sge::TransformComponent *cubeTransform;
	sge::ModelComponent *cubeComponent;
	sge::PhysicsComponent *cubePhysics;

	// Car:
	sge::Handle<sge::ModelResource> carHandle;
	sge::Entity *carEntity;
	sge::TransformComponent *carTransform;
	sge::ModelComponent *carComponent;

	// Room:
	sge::Handle<sge::ModelResource> roomHandle;
	sge::Entity *roomEntity;
	sge::TransformComponent *roomTransform;
	sge::ModelComponent *roomComponent;

	// Entity vectors:
	std::vector<sge::Entity*> gameObjects;
	std::vector<sge::Entity*> pointLights;
	std::vector<sge::Entity*> dirLights;

	// Misc:
	float alpha = 0;
	float movementSpeed = 5;
};