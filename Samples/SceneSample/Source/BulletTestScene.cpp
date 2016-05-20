#include "BulletTestScene.h"

#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"

#include "Core/Random.h"
#include "Game/TransformComponent.h"

#include"Audio/Audio.h"

#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"

#include "Game/PointLightComponent.h"
#include "Game/DirLightComponent.h"

// Mouse look sample
void BulletTestScene::mouseLook(int mouseX, int mouseY)
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

	float sensitivity = 0.15f;
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

void BulletTestScene::loadTextShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in);

	if (file.is_open())
	{
		std::cout << "Text shader opened!" << std::endl;

		std::stringstream stream;
		std::string str;

		stream << file.rdbuf();

		str = stream.str();

		std::copy(str.begin(), str.end(), std::back_inserter(data));

		data.push_back('\0');
	}
}

void BulletTestScene::loadBinaryShader(const std::string& path, std::vector<char>& data)
{
	std::ifstream file;

	file.open(path, std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open())
	{
		std::cout << "Binary shader opened!" << std::endl;
		data.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, std::ios::beg);
		file.read(data.data(), data.size());
		file.close();
	}
}

void BulletTestScene::spawnObject(sge::math::vec3 pos)
{
	EManager = new sge::EntityManager();

	sge::Entity* modentity = EManager->createEntity();

	sge::TransformComponent* modtransform = new sge::TransformComponent(modentity);
	modentity->setComponent(modtransform);

	sge::ModelComponent* modcomponent = new sge::ModelComponent(modentity);
	modcomponent->setShininess(15.0f);
	modentity->setComponent(modcomponent);

	modcomponent->setModelResource(&modelHandle2);
	modcomponent->setRenderer(engine->getRenderer());

	modentity->getComponent<sge::TransformComponent>()->setPosition(pos);
	modentity->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	modcomponent->setPipeline(pipelineNormals);

	// falling object
	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	fallRigidBodyCI.m_restitution = 1.0f;
	fallRigidBodyCI.m_friction = 0.5f;
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody);

	sge::MyPhysicsComponent* physcomponent = new sge::MyPhysicsComponent(modentity);
	physcomponent->setRigidBody(fallRigidBody);
	modentity->setComponent(physcomponent);

	// GameObject vector
	GameObjects.push_back(modentity);
}

BulletTestScene::BulletTestScene(sge::Spade* engine) : engine(engine), renderer(engine->getRenderer()), alpha(0.0f), useMouse(false), camSpeed(0.5f), played(false)
{
	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	//--------------
	// pipeline
	std::vector<char> pShaderDataNormals;
	std::vector<char> vShaderDataNormals;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/VertexShaderLights.cso", vShaderDataNormals);
	loadBinaryShader("../../Shaders/Compiled/PixelShaderLights.cso", pShaderDataNormals);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShaderLights.glsl", vShaderDataNormals);
	loadTextShader("../Assets/Shaders/PixelShaderLights.glsl", pShaderDataNormals);
#endif

	vertexShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderDataNormals.data(), vShaderDataNormals.size());
	pixelShader2 = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderDataNormals.data(), pShaderDataNormals.size());

	pipelineNormals = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader2, pixelShader2);
	engine->getRenderer()->getDevice()->bindPipeline(pipelineNormals);
	//--------------

	// Loading models
	modelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/simpleIcoSphere.dae");
    modelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandle2 = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	modelHandle2.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleFloor = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/floorSpecularNormal.dae");
	modelHandleFloor.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleTree = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/treeDiffuseSpecular.dae");
	modelHandleTree.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleEarth = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/earthDiffuseSpecular.dae");
	modelHandleEarth.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleRoom = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/RoomBoxBig.dae");
	modelHandleRoom.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	// Model
	EManager = new sge::EntityManager();

	modentity = EManager->createEntity();

	modtransform = new sge::TransformComponent(modentity);
	modentity->setComponent(modtransform);

	modcomponent = new sge::ModelComponent(modentity);
	modcomponent->setShininess(15.0f);
	modentity->setComponent(modcomponent);

	modcomponent->setModelResource(&modelHandle);
	modcomponent->setRenderer(engine->getRenderer());

	modentity->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	// Model 2
	modentity2 = EManager->createEntity();

	modtransform2 = new sge::TransformComponent(modentity2);
	modentity2->setComponent(modtransform2);

	modcomponent2 = new sge::ModelComponent(modentity2);
	modcomponent2->setShininess(256.0f);
	modentity2->setComponent(modcomponent2);

	modcomponent2->setModelResource(&modelHandle2);
    modcomponent2->setRenderer(engine->getRenderer());

	modentity2->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	// Floor
	modentityFloor = EManager->createEntity();

	modtransformFloor = new sge::TransformComponent(modentityFloor);
	modentityFloor->setComponent(modtransformFloor);

	modcomponentFloor = new sge::ModelComponent(modentityFloor);
	modcomponentFloor->setShininess(256.0f);
	modentityFloor->setComponent(modcomponentFloor);

	modcomponentFloor->setModelResource(&modelHandleFloor);
    modcomponentFloor->setRenderer(engine->getRenderer());

	modentityFloor->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityFloor->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 0.0f));

	modentityFloor->getComponent<sge::TransformComponent>()->setAngle(sge::math::radians(-90.0f));

	// Tree
	modentityTree = EManager->createEntity();

	modtransformTree = new sge::TransformComponent(modentityTree);
	modentityTree->setComponent(modtransformTree);

	modcomponentTree = new sge::ModelComponent(modentityTree);
	modcomponentTree->setShininess(2.0f);
	modentityTree->setComponent(modcomponentTree);

	modcomponentTree->setModelResource(&modelHandleTree);
	modcomponentTree->setRenderer(engine->getRenderer());

	modentityTree->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityTree->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 0.0f));

	modentityTree->getComponent<sge::TransformComponent>()->setAngle(sge::math::radians(-90.0f));

	// Earth
	modentityEarth = EManager->createEntity();

	modtransformEarth = new sge::TransformComponent(modentityEarth);
	modentityEarth->setComponent(modtransformEarth);

	modcomponentEarth = new sge::ModelComponent(modentityEarth);
	modcomponentEarth->setShininess(50.0f);
	modcomponentEarth->setGlossyness(1.0f);
	
	sge::Handle<sge::TextureResource> tex1;
	sge::Handle<sge::TextureResource> tex2;
	sge::Handle<sge::TextureResource> tex3;
	sge::Handle<sge::TextureResource> tex4;
	sge::Handle<sge::TextureResource> tex5;
	sge::Handle<sge::TextureResource> tex6;
	tex1 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/right.jpg");
	tex2 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/left.jpg");
	tex3 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/top.jpg");
	tex4 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/bottom.jpg");
	tex5 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/back.jpg");
	tex6 = sge::ResourceManager::getMgr().load<sge::TextureResource>("../Assets/CubeMap/front.jpg");
	
    sge::TextureResource* source[6];
	source[0] = tex2.getResource<sge::TextureResource>();
	source[1] = tex1.getResource<sge::TextureResource>();
	source[2] = tex4.getResource<sge::TextureResource>();
	source[3] = tex3.getResource<sge::TextureResource>();
	source[4] = tex6.getResource<sge::TextureResource>();
	source[5] = tex5.getResource<sge::TextureResource>();
	modcomponentEarth->setCubeMap(engine->getRenderer()->getDevice()->createCubeMap(source));
	modentityEarth->setComponent(modcomponentEarth);

	modcomponentEarth->setModelResource(&modelHandleEarth);
	modcomponentEarth->setRenderer(engine->getRenderer());

	modentityEarth->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityEarth->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	// Room
	modentityRoom = EManager->createEntity();

	modtransformRoom = new sge::TransformComponent(modentityRoom);
	modentityRoom->setComponent(modtransformRoom);

	modcomponentRoom = new sge::ModelComponent(modentityRoom);
	modcomponentRoom->setShininess(256.0f);
	modentityRoom->setComponent(modcomponentRoom);

	modcomponentRoom->setModelResource(&modelHandleRoom);
	modcomponentRoom->setRenderer(engine->getRenderer());

	modentityRoom->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityRoom->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 0.0f));

	modentityRoom->getComponent<sge::TransformComponent>()->setAngle(sge::math::radians(-90.0f));

    // Lights
    modentityLight = EManager->createEntity();

    modtransformLight = new sge::TransformComponent(modentityLight);
    modentityLight->setComponent(modtransformLight);

    modentityLight->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 4.0f, 0.0f));
    plcompo = new sge::PointLightComponent(modentityLight);
    modentityLight->setComponent(plcompo);

    sge::PointLight pointLight;
    pointLight.position = sge::math::vec4(0.0f);
    pointLight.constant = float(1.0);
    pointLight.mylinear = float(0.022);
    pointLight.quadratic = float(0.0019);
    pointLight.pad = 0.0f;
    pointLight.ambient = sge::math::vec4(0.0125, 0.05, 0.0125, 1.0);
    pointLight.diffuse = sge::math::vec4(0.2, 0.8, 0.2, 1.0);
    pointLight.specular = sge::math::vec4(0.25, 1.0, 0.25, 1.0);

    modentityLight->getComponent<sge::PointLightComponent>()->setLightData(pointLight);

	//
	modentityLight2 = EManager->createEntity();

	modtransformLight2 = new sge::TransformComponent(modentityLight2);
	modentityLight2->setComponent(modtransformLight2);

	modentityLight2->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 4.0f, 0.0f));
	plcompo2 = new sge::PointLightComponent(modentityLight2);
	modentityLight2->setComponent(plcompo2);

	sge::PointLight pointLight2;
	pointLight2.position = sge::math::vec4(0.0f);
	pointLight2.constant = float(1.0);
	pointLight2.mylinear = float(0.022);
	pointLight2.quadratic = float(0.0019);
	pointLight2.pad = 0.0f;
	pointLight2.ambient = sge::math::vec4(0.05, 0.0125, 0.0125, 1.0);
	pointLight2.diffuse = sge::math::vec4(0.8, 0.2, 0.2, 1.0);
	pointLight2.specular = sge::math::vec4(1.0, 0.25, 0.25, 1.0);

	modentityLight2->getComponent<sge::PointLightComponent>()->setLightData(pointLight2);

	//
	modentityLight3 = EManager->createEntity();

	modtransformLight3 = new sge::TransformComponent(modentityLight3);
	modentityLight3->setComponent(modtransformLight3);

	modentityLight3->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 4.0f, 0.0f));
	plcompo3 = new sge::PointLightComponent(modentityLight3);
	modentityLight3->setComponent(plcompo3);

	sge::PointLight pointLight3;
	pointLight3.position = sge::math::vec4(0.0f);
	pointLight3.constant = float(1.0);
	pointLight3.mylinear = float(0.022);
	pointLight3.quadratic = float(0.0019);
	pointLight3.pad = 0.0f;
	pointLight3.ambient = sge::math::vec4(0.0125, 0.0125, 0.05, 1.0);
	pointLight3.diffuse = sge::math::vec4(0.2, 0.2, 0.8, 1.0);
	pointLight3.specular = sge::math::vec4(0.25, 0.25, 1.0, 1.0);

	modentityLight3->getComponent<sge::PointLightComponent>()->setLightData(pointLight3);

	//
	modentityLight4 = EManager->createEntity();

	modtransformLight4 = new sge::TransformComponent(modentityLight4);
	modentityLight4->setComponent(modtransformLight3);

	modentityLight4->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(3.0f, 4.0f, 0.0f));
	plcompo4 = new sge::PointLightComponent(modentityLight4);
	modentityLight4->setComponent(plcompo4);

	sge::PointLight pointLight4;
	pointLight4.position = sge::math::vec4(0.0f, 40.0f, 0.0f, 1.0f);
	pointLight4.constant = float(1.0);
	pointLight4.mylinear = float(0.0022);
	pointLight4.quadratic = float(0.0007);
	pointLight4.pad = 0.0f;
	pointLight4.ambient = sge::math::vec4(0.05, 0.05, 0.05, 1.0);
	pointLight4.diffuse = sge::math::vec4(0.8, 0.8, 0.8, 1.0);
	pointLight4.specular = sge::math::vec4(0.5, 0.5, 0.5, 1.0);

	modentityLight4->getComponent<sge::PointLightComponent>()->setLightData(pointLight4);

   // dlcompo = new sge::DirLightComponent(modentityLight);
   // modentityLight->setComponent(dlcompo);

	// Set Pipelines
	modcomponent->setPipeline(pipelineNormals);
	modcomponent2->setPipeline(pipelineNormals);
	modcomponentFloor->setPipeline(pipelineNormals);
	modcomponentTree->setPipeline(pipelineNormals);
	modcomponentEarth->setPipeline(pipelineNormals);
	modcomponentRoom->setPipeline(pipelineNormals);

	modelHandle.getResource<sge::ModelResource>()->createBuffers();
	modelHandle2.getResource<sge::ModelResource>()->createBuffers();
	modelHandleFloor.getResource<sge::ModelResource>()->createBuffers();
	modelHandleTree.getResource<sge::ModelResource>()->createBuffers();
	modelHandleEarth.getResource<sge::ModelResource>()->createBuffers();
	modelHandleRoom.getResource<sge::ModelResource>()->createBuffers();

	// Bullet test
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// !!!!! Warning
	btConvexHullShape* fallShapeSuzanne = new btConvexHullShape();

	for (int i = 0; i < modelHandle.getResource<sge::ModelResource>()->getMeshes().size(); i++)
	{
		for (int j = 0; j < modelHandle.getResource<sge::ModelResource>()->getMeshes()[i]->vertices.size(); j++)
		{
			sge::math::vec3 vertexPos = sge::math::vec3(modelHandle.getResource<sge::ModelResource>()->getMeshes()[i]->vertices[j].Position.x, modelHandle.getResource<sge::ModelResource>()->getMeshes()[i]->vertices[j].Position.y, modelHandle.getResource<sge::ModelResource>()->getMeshes()[i]->vertices[j].Position.z);
			btVector3 position(btScalar(vertexPos.x), btScalar(vertexPos.y), btScalar(vertexPos.z));
			fallShapeSuzanne->addPoint(position);
		}
	}	
	
	//create a hull approximation
	btShapeHull* hull = new btShapeHull(fallShapeSuzanne);
	btScalar margin = fallShapeSuzanne->getMargin();
	hull->buildHull(margin);
	simplifiedConvexShape = new btConvexHullShape();
	
	for (int i = 0; i < hull->numVertices(); i++)
	{
		simplifiedConvexShape->addPoint(hull->getVertexPointer()[i]);
	}
	// !!!!! Warning


	groundShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));
	topShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));

	wall1Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall2Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));
	wall3Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall4Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));
	// Floor
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector4(0, 0, 1,1));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	// Top
	btDefaultMotionState* topMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 51, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		topRigidBodyCI(0, topMotionState, topShape, btVector4(0, 0, 1, 1));
	topRigidBody = new btRigidBody(topRigidBodyCI);
	dynamicsWorld->addRigidBody(topRigidBody);

	// Wall 1
	btDefaultMotionState* wall1MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(51, 51, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wall1RigidBodyCI(0, wall1MotionState, wall1Shape, btVector4(0, 0, 1, 1));
	wall1RigidBody = new btRigidBody(wall1RigidBodyCI);
	dynamicsWorld->addRigidBody(wall1RigidBody);

	// Wall 2
	btDefaultMotionState* wall2MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 51, -51)));
	btRigidBody::btRigidBodyConstructionInfo
		wall2RigidBodyCI(0, wall2MotionState, wall2Shape, btVector4(0, 0, 1, 1));
	wall2RigidBody = new btRigidBody(wall2RigidBodyCI);
	dynamicsWorld->addRigidBody(wall2RigidBody);

	// Wall 3
	btDefaultMotionState* wall3MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-51, 51, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		wall3RigidBodyCI(0, wall3MotionState, wall3Shape, btVector4(0, 0, 1, 1));
	wall3RigidBody = new btRigidBody(wall3RigidBodyCI);
	dynamicsWorld->addRigidBody(wall3RigidBody);

	// Wall 4
	btDefaultMotionState* wall4MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 51, 51)));
	btRigidBody::btRigidBodyConstructionInfo
		wall4RigidBodyCI(0, wall4MotionState, wall4Shape, btVector4(0, 0, 1, 1));
	wall4RigidBody = new btRigidBody(wall4RigidBodyCI);
	dynamicsWorld->addRigidBody(wall4RigidBody);

	// Tree
	btCylinderShape* treeShape = new btCylinderShape(btVector3(btScalar(0.4), btScalar(15.0), btScalar(0.4)));

	btDefaultMotionState* TreeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		treeRigidBodyCI(0, TreeMotionState, treeShape, btVector4(0, 0, 1, 1));
	btRigidBody* treeRigidBody = new btRigidBody(treeRigidBodyCI);
	dynamicsWorld->addRigidBody(treeRigidBody);

	//sge::MyPhysicsComponent* physcomponentTree = new sge::MyPhysicsComponent(modentityTree);
	//physcomponentTree->setRigidBody(treeRigidBody);
	//modentityTree->setComponent(physcomponentTree);

	// Earth
	btSphereShape* sphereShape = new btSphereShape(btScalar(9.5f));
	modentityEarth->getComponent<sge::TransformComponent>()->setScale(sge::math::vec3(10.0f));

	btDefaultMotionState* EarthMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(20, 15, 0)));
	btScalar massE = 10;
	btVector3 fallInertiaE(0, 0, 0);
	sphereShape->calculateLocalInertia(10.0f, fallInertiaE);
	btRigidBody::btRigidBodyConstructionInfo
		earthRigidBodyCI(massE, EarthMotionState, sphereShape, fallInertiaE);
	earthRigidBodyCI.m_restitution = 1.0f;
	earthRigidBodyCI.m_rollingFriction = 0.5f;
	btRigidBody* earthRigidBody = new btRigidBody(earthRigidBodyCI);
	earthRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(earthRigidBody);

	sge::MyPhysicsComponent* physcomponentEarth = new sge::MyPhysicsComponent(modentityEarth);
	physcomponentEarth->setRigidBody(earthRigidBody);
	modentityEarth->setComponent(physcomponentEarth);

	// falling object
	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2, 23, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	simplifiedConvexShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, simplifiedConvexShape, fallInertia);
	fallRigidBodyCI.m_restitution = 1.0f;
	fallRigidBodyCI.m_friction = 0.5f;
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody);

	sge::MyPhysicsComponent* physcomponent = new sge::MyPhysicsComponent(modentity);
	physcomponent->setRigidBody(fallRigidBody);
	modentity->setComponent(physcomponent);

	// falling object 2
	fallShape = new btBoxShape(btVector3(1, 1, 1));

	btDefaultMotionState* fallMotionState2 =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 23, 0)));
	btScalar mass2 = 1;
	btVector3 fallInertia2(0, 0, 0);
	fallShape->calculateLocalInertia(mass2, fallInertia2);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass2, fallMotionState2, fallShape, fallInertia2);
	fallRigidBodyCI2.m_restitution = 0.4f;
	fallRigidBodyCI2.m_friction = 0.8f;
	fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
	fallRigidBody2->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody2);

	sge::MyPhysicsComponent* physcomponent1 = new sge::MyPhysicsComponent(modentity2);
	physcomponent1->setRigidBody(fallRigidBody2);
	modentity2->setComponent(physcomponent1);

	// GameObject vector
	GameObjects.push_back(modentity);
	GameObjects.push_back(modentity2);
	GameObjects.push_back(modentityRoom);
	GameObjects.push_back(modentityFloor);
	GameObjects.push_back(modentityTree);
	GameObjects.push_back(modentityEarth);

	sge::Entity* camentity = EManager->createEntity();
    sge::Entity* camentity2 = EManager->createEntity();

	camtransform = new sge::TransformComponent(camentity);
	camentity->setComponent(camtransform);

	camcomponent = new sge::CameraComponent(camentity);
	camentity->setComponent(camcomponent);

	cameraPos = glm::vec3(5.0f, 10.0f, 48.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    camcomponent->setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camcomponent->setViewport(0, 0, 1280, 720);
    camtransform->setPosition(cameraPos);
    camtransform->setFront(cameraFront);
    camtransform->setUp(cameraUp);

    camtransform2 = new sge::TransformComponent(camentity2);
    camentity2->setComponent(camtransform2);

    camcomponent2 = new sge::CameraComponent(camentity2);
    camentity2->setComponent(camcomponent2);

	sge::math::vec3 cameraPos2 = glm::vec3(5.0f, 10.0f, 48.0f);
	sge::math::vec3 cameraFront2 = glm::vec3(0.0f, 0.0f, -1.0f);
	sge::math::vec3 cameraUp2 = glm::vec3(0.0f, 1.0f, 0.0f);

    camcomponent2->setPerspective(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camcomponent2->setViewport(1280-420, 720-280, 320, 180);
    camtransform2->setPosition(cameraPos2);
    camtransform2->setFront(cameraFront2);
    camtransform2->setUp(cameraUp2);

    cameras.push_back(camentity);
    cameras.push_back(camentity2);
}

BulletTestScene::~BulletTestScene()
{
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(fallRigidBody2);
	delete fallRigidBody2->getMotionState();
	delete fallRigidBody2;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	dynamicsWorld->removeRigidBody(topRigidBody);
	delete topRigidBody->getMotionState();
	delete topRigidBody;

	dynamicsWorld->removeRigidBody(wall1RigidBody);
	delete wall1RigidBody->getMotionState();
	delete wall1RigidBody;
	dynamicsWorld->removeRigidBody(wall2RigidBody);
	delete wall2RigidBody->getMotionState();
	delete wall2RigidBody;
	dynamicsWorld->removeRigidBody(wall3RigidBody);
	delete wall3RigidBody->getMotionState();
	delete wall3RigidBody;
	dynamicsWorld->removeRigidBody(wall4RigidBody);
	delete wall4RigidBody->getMotionState();
	delete wall4RigidBody;

	delete fallShape;

	delete groundShape;
	delete topShape;

	delete wall1Shape;
	delete wall2Shape;
	delete wall3Shape;
	delete wall4Shape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	sge::ResourceManager::getMgr().release(modelHandle);
	sge::ResourceManager::getMgr().release(modelHandle2);
	sge::ResourceManager::getMgr().release(modelHandleFloor);
	sge::ResourceManager::getMgr().release(modelHandleTree);
	sge::ResourceManager::getMgr().release(modelHandleEarth);

	//engine->getRenderer()->getDevice()->debindPipeline(pipeline);
	engine->getRenderer()->getDevice()->debindPipeline(pipelineNormals);


	//engine->getRenderer()->getDevice()->deleteShader(vertexShader);
	//engine->getRenderer()->getDevice()->deleteShader(pixelShader);

	//engine->getRenderer()->getDevice()->deletePipeline(pipeline);
}

void BulletTestScene::update(float step)
{
	dynamicsWorld->stepSimulation(step, 10);

	//------------------------------------------------
	// CameraControls
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_F1))
	{
		useMouse = true;
		if (useMouse) SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_F2))
	{
		useMouse = false;
		if (!useMouse) SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_UP))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp + cameraFront*camSpeed);
		}
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_DOWN))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp - cameraFront*camSpeed);
		}
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_LEFT))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp - sge::math::cross(frontTemp, upTemp)*camSpeed);
		}
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_RIGHT))
	{
		if (useMouse == true)
		{
			sge::math::vec3 temp = cameras[0]->getComponent<sge::TransformComponent>()->getPosition();
			sge::math::vec3 frontTemp = cameras[0]->getComponent<sge::TransformComponent>()->getFront();
			sge::math::vec3 upTemp = cameras[0]->getComponent<sge::TransformComponent>()->getUp();
			cameras[0]->getComponent<sge::TransformComponent>()->setPosition(temp + sge::math::cross(frontTemp, upTemp)*camSpeed);
		}
	}


	if (useMouse)
	{
#ifdef _WIN32
		engine->mouseInput->getRelativeMouseState(&mouseXpos, &mouseYpos);

		mouseLook(mouseXpos, mouseYpos);
#endif
		cameras[0]->getComponent<sge::TransformComponent>()->setFront(cameraFront);
	}
	//------------------------------------------------
	
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_3) && played == true)
	{
		played = false;
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_4))
	{
		mixer.stop();
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_2) && played == false)
	{
		mixer.play("../Assets/Audio/scifi.flac");
		played = true;
	}		

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_SPACE))
	{
        btScalar randomx = (btScalar)sge::random(10, 500);
        btScalar randomy = (btScalar)sge::random(10, 500);
        btScalar randomz = (btScalar)sge::random(10, 500);
		//fallRigidBody->applyCentralImpulse(btVector3(0, 10, 0));
		fallRigidBody->applyTorque(btVector3(randomx, randomy, randomz));
		
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_1))
	{
		btScalar randomx = (btScalar)sge::random(0, 99) - 49;
		btScalar randomy = (btScalar)sge::random(5, 49);
		btScalar randomz = (btScalar)sge::random(0, 99) - 49;
		spawnObject(sge::math::vec3(randomx, randomy, randomz));
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_P))
	{
        btScalar randomx = (btScalar)sge::random(-10, 10);
        btScalar randomy = (btScalar)sge::random(-10, 10);
        btScalar randomz = (btScalar)sge::random(-10, 10);
		dynamicsWorld->setGravity(btVector3(randomx, randomy, randomz));
		
	}

	// Physics test with second object
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_A))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(-2, 0, 0));
		
		
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_D))
	{
		fallRigidBody2->applyCentralImpulse(btVector3( 2, 0, 0));

		
	}
	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_W))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(0, -0, -2));

		
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_S))
	{
		fallRigidBody2->applyCentralImpulse(btVector3(0, 0, 2));

		
	}

	for (int i = 0; i < GameObjects.size(); i++)
	{
		if (GameObjects[i]->getComponent<sge::MyPhysicsComponent>() != NULL)
		{
			GameObjects[i]->getComponent<sge::MyPhysicsComponent>()->update();
		}		
	}

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	alpha += 0.01f;
	float x = 15.0f*cos(alpha);
	float z = 15.0f*sin(alpha);

    modentityLight->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(x, 4.0f, z));
    modentityLight->getComponent<sge::PointLightComponent>()->update();

	modentityLight2->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(z, 5.0f, x));
	modentityLight2->getComponent<sge::PointLightComponent>()->update();

	modentityLight3->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(-z, 6.0f, -x));
	modentityLight3->getComponent<sge::PointLightComponent>()->update();

    for (auto camera : cameras)
    {
        camera->getComponent<sge::CameraComponent>()->update();
    }
}
void BulletTestScene::draw()
{
    renderer->addCameras(1, &cameras.front());
    renderer->addCameras(1, &cameras.back());
    renderer->begin();
    
	renderer->renderModels(GameObjects.size(), GameObjects.data());
	
    renderer->renderLights(1, &modentityLight);
	renderer->renderLights(1, &modentityLight2);
	renderer->renderLights(1, &modentityLight3);
	renderer->renderLights(1, &modentityLight4);

    renderer->end();
    renderer->render();

    renderer->present();
    renderer->clear();
}

void BulletTestScene::interpolate(float alpha)
{
}
