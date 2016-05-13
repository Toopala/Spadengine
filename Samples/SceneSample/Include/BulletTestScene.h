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
#include "Game/TransformComponent.h"

#include "Game/Component.h"
#include "Audio/Audio.h"

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

	// Audio test
	sge::Audio mixer;
	//mixer.stop();

	///MouseLook
	bool useMouse;
	void mouseLook(int x, int y);
	float lastX, lastY;
	float yaw, pitch;
	int mouseXpos, mouseYpos;
	int mousseX, mousseY;
	bool firstMouse = true;
	float camSpeed;
	///

	sge::Spade* engine;
    sge::RenderSystem* renderer;

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
	sge::Pipeline* pipelineNormals;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader;
	sge::Shader* pixelShader;
	sge::Shader* vertexShader2;
	sge::Shader* pixelShader2;
	sge::Texture* texture;
	sge::Texture* texture2;

	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	UniformData2 uniformData2;

	sge::Handle <sge::ModelResource> modelHandle;
	sge::Handle <sge::ModelResource> modelHandle2;
	sge::Handle <sge::ModelResource> modelHandleFloor;
	sge::Handle <sge::ModelResource> modelHandleTree;
	sge::Handle <sge::ModelResource> modelHandleTreeLeaves;
	sge::Handle <sge::ModelResource> modelHandleRoom;

	sge::EntityManager* EManager;
    std::vector<sge::Entity*> cameras;
	sge::TransformComponent* camtransform;
    sge::TransformComponent* camtransform2;
	sge::CameraComponent* camcomponent;
    sge::CameraComponent* camcomponent2;

	sge::Entity* modentity;
	sge::Entity* modentity2;
	sge::Entity* modentityFloor;
	sge::Entity* modentityTree;
	sge::Entity* modentityTreeLeaves;
	sge::Entity* modentityRoom;

    sge::Entity* modentityLight;
	sge::Entity* modentityLight2;
	sge::Entity* modentityLight3;

	sge::TransformComponent* modtransform;
	sge::TransformComponent* modtransform2;
	sge::TransformComponent* modtransformFloor;
	sge::TransformComponent* modtransformTree;
	sge::TransformComponent* modtransformTreeLeaves;
	sge::TransformComponent* modtransformRoom;

    sge::TransformComponent* modtransformLight;
	sge::TransformComponent* modtransformLight2;
	sge::TransformComponent* modtransformLight3;

	sge::ModelComponent* modcomponent;
	sge::ModelComponent* modcomponent2;
	sge::ModelComponent* modcomponentFloor;
	sge::ModelComponent* modcomponentTree;
	sge::ModelComponent* modcomponentTreeLeaves;
	sge::ModelComponent* modcomponentRoom;

    sge::PointLightComponent* plcompo;
	sge::PointLightComponent* plcompo2;
	sge::PointLightComponent* plcompo3;
    sge::DirLightComponent* dlcompo;

	std::vector<sge::Entity*> GameObjects;

	float alpha;

	btConvexHullShape* simplifiedConvexShape;
	void spawnObject(sge::math::vec3 pos);

	bool played;
};

namespace sge
{
	class TransformComponent;

	class MyPhysicsComponent : public Component
	{
	public:
		MyPhysicsComponent(Entity* ent) : Component(ent), transform(nullptr), body(nullptr)
		{
			transform = getParent()->getComponent<TransformComponent>();

			SGE_ASSERT(transform);
		}

		void update()
		{
			btTransform trans;
			if (body != nullptr)
			{
				body->getMotionState()->getWorldTransform(trans);

				getParent()->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
				getParent()->getComponent<sge::TransformComponent>()->setAngle(trans.getRotation().getAngle());
				getParent()->getComponent<sge::TransformComponent>()->setRotationVector(sge::math::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
			}			
		};

		void setRigidBody(btRigidBody* body)
		{
			this->body = body;
		}
	private:
		btRigidBody* body;
		TransformComponent* transform;
	};
}

