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

BulletTestScene::BulletTestScene(sge::Spade* engine) : engine(engine), renderer(engine->getRenderer()), alpha(0.0f), useMouse(false), camSpeed(0.5f)
{
	std::vector<char> pShaderData;
	std::vector<char> vShaderData;

	// Audio test
	sge::Audio mixer;

#ifdef DIRECTX11
	loadBinaryShader("../../Shaders/Compiled/VertexShaderLights.cso", vShaderData);
	loadBinaryShader("../../Shaders/Compiled/PixelShaderLights.cso", pShaderData);
#elif OPENGL4
	loadTextShader("../Assets/Shaders/VertexShaderLightsNoNormalTexture.glsl", vShaderData);
	loadTextShader("../Assets/Shaders/PixelShaderLightsNoNormalTexture.glsl", pShaderData);
#endif

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	vertexShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	pixelShader = engine->getRenderer()->getDevice()->createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	pipeline = engine->getRenderer()->getDevice()->createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	engine->getRenderer()->getDevice()->bindPipeline(pipeline);

	//--------------
	// New pipeline
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

	//Assimp test
	modelHandle = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/diamondDiffuseSpecular.dae");
    modelHandle.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandle2 = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/cubeSpecularNormal.dae");
	modelHandle2.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleFloor = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/floorSpecularNormal.dae");
	modelHandleFloor.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleTree = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/treeDiffuseSpecular.dae");
	modelHandleTree.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleTreeLeaves = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/treeLeavesDiffuseSpecular.dae");
	modelHandleTreeLeaves.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	modelHandleRoom = sge::ResourceManager::getMgr().load<sge::ModelResource>("../Assets/RoomBoxBig.dae");
	modelHandleRoom.getResource<sge::ModelResource>()->setDevice(engine->getRenderer()->getDevice());

	EManager = new sge::EntityManager();

	modentity = EManager->createEntity();

	modtransform = new sge::TransformComponent(modentity);
	modentity->setComponent(modtransform);

	modcomponent = new sge::ModelComponent(modentity);
	modcomponent->setShininess(15.0f);
	modentity->setComponent(modcomponent);

	modcomponent->setModelResource(&modelHandle);
	modcomponent->setRenderer(engine->getRenderer());

	modentity->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 23.0f, 0.0f));
	modentity->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	modentity2 = EManager->createEntity();

	modtransform2 = new sge::TransformComponent(modentity2);
	modentity2->setComponent(modtransform2);

	modcomponent2 = new sge::ModelComponent(modentity2);
	modcomponent2->setShininess(256.0f);
	modentity2->setComponent(modcomponent2);

	modcomponent2->setModelResource(&modelHandle2);
    modcomponent2->setRenderer(engine->getRenderer());

	modentity2->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(5.0f, 23.0f, 0.0f));
	modentity2->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(0.0f, 0.0f, 1.0f));

	modentityFloor = EManager->createEntity();

	modtransformFloor = new sge::TransformComponent(modentityFloor);
	modentityFloor->setComponent(modtransformFloor);

	modcomponentFloor = new sge::ModelComponent(modentityFloor);
	modcomponentFloor->setShininess(100.0f);
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

	// Tree leaves
	modentityTreeLeaves = EManager->createEntity();

	modtransformTreeLeaves = new sge::TransformComponent(modentityTreeLeaves);
	modentityTreeLeaves->setComponent(modtransformTreeLeaves);

	modcomponentTreeLeaves = new sge::ModelComponent(modentityTreeLeaves);
	modcomponentTreeLeaves->setShininess(256.0f);
	modentityTreeLeaves->setComponent(modcomponentTreeLeaves);

	modcomponentTreeLeaves->setModelResource(&modelHandleTreeLeaves);
	modcomponentTreeLeaves->setRenderer(engine->getRenderer());

	modentityTreeLeaves->getComponent<sge::TransformComponent>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	modentityTreeLeaves->getComponent<sge::TransformComponent>()->setRotationVector(glm::vec3(1.0f, 0.0f, 0.0f));

	modentityTreeLeaves->getComponent<sge::TransformComponent>()->setAngle(sge::math::radians(-90.0f));

	modcomponent->setPipeline(pipelineNormals);
	modcomponent2->setPipeline(pipelineNormals);
	modcomponentFloor->setPipeline(pipelineNormals);
	modcomponentTree->setPipeline(pipeline);
	modcomponentTreeLeaves->setPipeline(pipeline);

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

    // Tree leaves
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

    dlcompo = new sge::DirLightComponent(modentityLight);
    modentityLight->setComponent(dlcompo);

	modcomponent->setPipeline(pipelineNormals);
	modcomponent2->setPipeline(pipelineNormals);
	modcomponentFloor->setPipeline(pipelineNormals);
	modcomponentTree->setPipeline(pipeline);
	modcomponentTreeLeaves->setPipeline(pipeline);
	modcomponentRoom->setPipeline(pipelineNormals);

	modelHandle.getResource<sge::ModelResource>()->createBuffers();
	modelHandle2.getResource<sge::ModelResource>()->createBuffers();
	modelHandleFloor.getResource<sge::ModelResource>()->createBuffers();
	modelHandleTree.getResource<sge::ModelResource>()->createBuffers();
	modelHandleTreeLeaves.getResource<sge::ModelResource>()->createBuffers();
	modelHandleRoom.getResource<sge::ModelResource>()->createBuffers();

	// GameObject vector
	GameObjects.push_back(modentity);
	GameObjects.push_back(modentity2);
	GameObjects.push_back(modentityRoom);
	GameObjects.push_back(modentityFloor);
	GameObjects.push_back(modentityTree);
	GameObjects.push_back(modentityTreeLeaves);

	// Bullet test
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	groundShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));
	topShape = new btBoxShape(btVector3(btScalar(51.), btScalar(1.), btScalar(51.)));
	
	wall1Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall2Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));
	wall3Shape = new btBoxShape(btVector3(btScalar(1.), btScalar(51.), btScalar(51.)));
	wall4Shape = new btBoxShape(btVector3(btScalar(51.), btScalar(51.), btScalar(1.)));

	btCylinderShape* treeShape = new btCylinderShape(btVector3(btScalar(0.4), btScalar(15.0), btScalar(0.4)));
	
	fallShape = new btBoxShape(btVector3(1, 1, 1));

	// !!!!! Warning
	btConvexHullShape* fallShapeSuzanne = new btConvexHullShape();

	for (int j = 0; j < modelHandle.getResource<sge::ModelResource>()->getVerticeArray()->size(); j++)
	{
		sge::math::vec3 vertexPos = sge::math::vec3(modelHandle.getResource<sge::ModelResource>()->getVerticeArray()->at(j).Position.x, modelHandle.getResource<sge::ModelResource>()->getVerticeArray()->at(j).Position.y, modelHandle.getResource<sge::ModelResource>()->getVerticeArray()->at(j).Position.z);
		btVector3 position(btScalar(vertexPos.x), btScalar(vertexPos.y), btScalar(vertexPos.z));
		fallShapeSuzanne->addPoint(position);
	}
	
	//create a hull approximation
	btShapeHull* hull = new btShapeHull(fallShapeSuzanne);
	btScalar margin = fallShapeSuzanne->getMargin();
	hull->buildHull(margin);
	btConvexHullShape* simplifiedConvexShape = new btConvexHullShape();
	
	for (int i = 0; i < hull->numVertices(); i++)
	{
		simplifiedConvexShape->addPoint(hull->getVertexPointer()[i]);
	}

	// !!!!! Warning

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
	btDefaultMotionState* TreeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		treeRigidBodyCI(0, TreeMotionState, treeShape, btVector4(0, 0, 1, 1));
	btRigidBody* treeRigidBody = new btRigidBody(treeRigidBodyCI);
	dynamicsWorld->addRigidBody(treeRigidBody);

	// falling object
	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 23, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	simplifiedConvexShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, simplifiedConvexShape, fallInertia);
	fallRigidBodyCI.m_restitution = 1.0f;
	fallRigidBodyCI.m_friction = 0.5f;
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setActivationState(DISABLE_DEACTIVATION);
	dynamicsWorld->addRigidBody(fallRigidBody);
	

	// falling object 2
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
	sge::ResourceManager::getMgr().release(modelHandleTreeLeaves);

	engine->getRenderer()->getDevice()->debindPipeline(pipeline);
	engine->getRenderer()->getDevice()->debindPipeline(pipelineNormals);


	engine->getRenderer()->getDevice()->deleteShader(vertexShader);
	engine->getRenderer()->getDevice()->deleteShader(pixelShader);

	engine->getRenderer()->getDevice()->deletePipeline(pipeline);
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

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_SPACE))
	{
        btScalar randomx = (btScalar)sge::random(10, 500);
        btScalar randomy = (btScalar)sge::random(10, 500);
        btScalar randomz = (btScalar)sge::random(10, 500);
		//fallRigidBody->applyCentralImpulse(btVector3(0, 10, 0));
		fallRigidBody->applyTorque(btVector3(randomx, randomy, randomz));
		
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

	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);

	std::cout << "Box height: " << trans.getOrigin().getY() << std::endl;

	modentity->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	modentity->getComponent<sge::TransformComponent>()->setAngle(trans.getRotation().getAngle());
	modentity->getComponent<sge::TransformComponent>()->setRotationVector(sge::math::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));

	btTransform trans2;
	fallRigidBody2->getMotionState()->getWorldTransform(trans2);
	modentity2->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(trans2.getOrigin().getX(), trans2.getOrigin().getY(), trans2.getOrigin().getZ()));
	modentity2->getComponent<sge::TransformComponent>()->setAngle(trans2.getRotation().getAngle());
	modentity2->getComponent<sge::TransformComponent>()->setRotationVector(sge::math::vec3(trans2.getRotation().getAxis().getX(), trans2.getRotation().getAxis().getY(), trans2.getRotation().getAxis().getZ()));

	if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
	{
		engine->stop();
	}

	//if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_F1))
	//{
	//	camentity->getComponent<sge::CameraComponent>()->enableMouse();
	//}
	//if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_F2))
	//{
	//	camentity->getComponent<sge::CameraComponent>()->disableMouse();
	//}
	alpha += 0.01f;
	float x = 15.0f*cos(alpha);
	float z = 15.0f*sin(alpha);

    modentityLight->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(x, 5.0f, z));
    modentityLight->getComponent<sge::PointLightComponent>()->update();

	//camentity->getComponent<sge::TransformComponent>()->setPosition(sge::math::vec3(x,0.0f, z));

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
    
	for (int i = 0; i < GameObjects.size(); i++)
	{
		renderer->renderModels(1, &GameObjects[i]);
	}

    renderer->renderLights(1, &modentityLight);

    renderer->end();
    renderer->render();

    renderer->present();
    renderer->clear();
}

void BulletTestScene::interpolate(float alpha)
{
}
