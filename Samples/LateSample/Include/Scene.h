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

#include "Resources/FontResource.h"
#include "Game/TextComponent.h"

// FORWARD DECLARE
struct sge::Pipeline;
struct sge::Buffer;
struct sge::Viewport;
struct sge::Shader;
struct sge::Texture;

class Scene : public sge::Scene
{
public:
	Scene(sge::Spade* engine);
	~Scene();

	void update(float dt);
	void draw();
	void interpolate(float alpha);

	void setCubeMap(sge::ModelComponent *component, sge::math::ivec2 size, std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back);
	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);
	void mouseLook();
	sge::Entity* createText(float x, float y, const std::string& text);

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

	// Text:
	sge::Entity *textEntity;
	sge::Handle<sge::FontResource> fontResource;
	sge::ComponentFactory<sge::TransformComponent> transformFactory;
	sge::ComponentFactory<sge::TextComponent> textFactory;

	// Models:

	// Cube:
	sge::Handle<sge::ModelResource> cubeHandle;
	sge::Entity *cubeEntity;
	sge::TransformComponent *cubeTransform;
	sge::ModelComponent *cubeComponent;

	// Deer:
	sge::Handle<sge::ModelResource> deerHandle;
	sge::Entity *deerEntity;
	sge::TransformComponent *deerTransform;
	sge::ModelComponent *deerComponent;

	// Diamond:
	sge::Handle<sge::ModelResource> diamondHandle;
	sge::Entity *diamondEntity;
	sge::TransformComponent *diamondTransform;
	sge::ModelComponent *diamondComponent;

	// Room:
	sge::Handle<sge::ModelResource> roomHandle;
	sge::Entity *roomEntity;
	sge::TransformComponent *roomTransform;
	sge::ModelComponent *roomComponent;

	// Entity vectors:
	std::vector<sge::Entity*> gameObjects;
	std::vector<sge::Entity*> pointLights;
	std::vector<sge::Entity*> dirLights;
	std::vector<sge::Entity*> texts;

	// Misc:
	float alpha = 0;
	float movementSpeed = 1;
};