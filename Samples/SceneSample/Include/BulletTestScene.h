#pragma once

#include "Renderer/Viewport.h"
#include "Spade/Spade.h"
#include "Game/Scene.h"
#include "Core/Math.h"
#include "Resources/ModelResource.h"

#include <Bullet/btBulletDynamicsCommon.h>

#include "Game/EntityManager.h"
#include "Game/SystemManager.h"
#include "Game/CameraComponent.h"
#include "Game/ModelComponent.h"
#include "Game/ModelRenderingSystem.h"
#include "Game/TransformComponent.h"

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

class BulletTestScene : public sge::Scene
{
public:
	BulletTestScene(sge::Spade* engine);
	~BulletTestScene();

	void update(float step);
	void interpolate(float alpha);
	void draw();

	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);
private:
	sge::Viewport viewport;
	sge::Spade* engine;

	btDiscreteDynamicsWorld* dynamicsWorld;
	btRigidBody* fallRigidBody;
	btRigidBody* fallRigidBody2;
	btRigidBody* groundRigidBody;
	btRigidBody* wall1RigidBody;
	btRigidBody* wall2RigidBody;
	btRigidBody* wall3RigidBody;
	btRigidBody* wall4RigidBody;
	btRigidBody* topRigidBody;

	btCollisionShape* groundShape;
	btCollisionShape* topShape;

	btCollisionShape* wall1Shape;
	btCollisionShape* wall2Shape;
	btCollisionShape* wall3Shape;
	btCollisionShape* wall4Shape;

	btCollisionShape* fallShape;

	btSequentialImpulseConstraintSolver* solver;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;

	// For 3d object

	glm::vec3 cameraFront;
	sge::math::mat4 V;
	sge::math::mat4 P;

	sge::Pipeline* pipeline;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader;
	sge::Shader* pixelShader;
	sge::Texture* texture;
	sge::Texture* texture2;

	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	UniformData2 uniformData2;

	sge::Handle <sge::ModelResource> modelHandle;
	sge::Handle <sge::ModelResource> modelHandleFloor;

	sge::EntityManager* EManager;
	sge::Entity* camentity;
	sge::TransformComponent* camtransform;
	sge::CameraComponent* camcomponent;

	sge::Entity* modentity;
	sge::Entity* modentity2;
	sge::Entity* modentityFloor;
	sge::TransformComponent* modtransform;
	sge::TransformComponent* modtransform2;
	sge::TransformComponent* modtransformFloor;
	sge::ModelComponent* modcomponent;
	sge::ModelComponent* modcomponent2;
	sge::ModelComponent* modcomponentFloor;

	sge::ModelRenderingSystem* modelSystem;

	float alpha;
};
