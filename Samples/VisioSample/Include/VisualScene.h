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

class VisualScene : public sge::Scene
{
public:
	VisualScene(sge::Spade* engine);
	~VisualScene();

	void update(float step);
	void interpolate(float alpha);
	void draw();

	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);
	void mouseLook(int mouseX, int mouseY);
private:
	sge::Spade *engine;
	sge::RenderSystem *renderer;
	
	// For 3d object
	sge::math::mat4 V;
	sge::math::mat4 P;

	sge::Pipeline* pipeline;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader;
	sge::Shader* pixelShader;
	sge::Shader* vertexShader2;
	sge::Shader* pixelShader2;
	sge::Texture* texture;
	sge::Texture* texture2;

	// Handle
	sge::Handle <sge::ModelResource> modelHandleCube, modelHandleCube2, modelHandleCube3, modelHandleRoom;
									
	// Entity
	sge::EntityManager* EManager;
	sge::Entity* modentityCube, *modentityCube2, *modentityCube3, *modentityRoom, *modentityLight, *modentityLight2;

	sge::TransformComponent *modtransformCube, *modtransformCube2, *modtransformCube3, *modtransformRoom, *modtransformLight, *modtransformLight2;

	sge::ModelComponent* modComponentCube,*modComponentCube2, *modComponentCube3, *modComponentRoom, *modComponentLight, *modComponentLight2;


	// Light components
	sge::PointLightComponent *pointLightComp, *pointLightComp2;

	// Vector for game objects
	std::vector<sge::Entity*> gameObjects;


	// Camera
	std::vector<sge::Entity*> cameras;
	sge::Entity* screenCamera;
	sge::CameraComponent* camcomponent;
	sge::TransformComponent* camtransform;

	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	// Mouse look
	bool useMouse;
	float lastX, lastY;
	float yaw, pitch;
	int mouseXpos, mouseYpos;
	int mousseX, mousseY;
	bool firstMouse = true;
	float camSpeed;

	float alpha = 0.0f;
	float rotate = 0.0f;
};