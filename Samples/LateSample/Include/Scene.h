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

private:
	// Engine:
	sge::Spade *engine;
	sge::RenderSystem *renderer;
	sge::EntityManager *entityManager;

	// Pipeline:
	sge::Pipeline *pipeline;
	sge::Pipeline *pipelineNormals;

	// Buffer:
	sge::Buffer *vertexBuffer;
	sge::Buffer *uniformBuffer;

	// Shader:
	sge::Shader *vertexShader;
	sge::Shader *pixelShader;

	// Light:
	sge::Entity *pointLightEntity;
	sge::TransformComponent *pointLightTransform;
	sge::ModelComponent *pointLightModel;
	sge::PointLightComponent *pointLightComponent;

	// Texture:
	sge::Texture* texture;
	sge::Texture* texture2;

	// Camera:
	std::vector<sge::Entity*> cameras;
	sge::TransformComponent* cameraTransform;
	sge::CameraComponent *cameraComponent;

	// Mouse:
	sge::math::ivec2 mouse;

	// Model:
	sge::Handle<sge::ModelResource> cubeModelHandle;
	sge::Entity *cubeModel;
	sge::TransformComponent *cubeTransform;
	sge::ModelComponent *cubeComponent;

	// Game objects:
	std::vector<sge::Entity*> gameObjects;
};