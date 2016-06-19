#include <vector>

#include "Game/Scene.h"
#include "Game/EntityManager.h"
#include "Game/ComponentFactory.h"
#include "Game/TransformComponent.h"
#include "Game/SpriteComponent.h"
#include "Game/CameraComponent.h"
#include "Game/TextComponent.h"
#include "Game/PhysicsSystem.h"
#include "Game/ModelComponent.h"
#include "Game/DirLightComponent.h"
#include "Game/LightComponent.h"
#include "Game/PointLightComponent.h"
#include "Game/SpotLightComponent.h"
#include "Game/RenderSystem.h"

#include "Resources/ResourceManager.h"
#include "Resources/TextureResource.h"
#include "Resources/FontResource.h"
#include "Resources/ModelResource.h"
#include "Core/Math.h"
#include "Renderer/Viewport.h"


// Forward declares
namespace sge
{
    class Spade;
    class Entity;
    struct Texture;
    struct RenderTarget;

	struct sge::Pipeline;
	struct sge::Buffer;
	struct sge::Viewport;
	struct sge::Shader;
	
	struct Vertex;
}

struct UniformData2
{
	sge::math::mat4 PV;
	sge::math::mat4 M;
};

class GameScene : public sge::Scene
{
public:
    GameScene(sge::Spade* engine);
    ~GameScene();

    void update(float step);
    void interpolate(float alpha);
    void draw();

	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);
	void  mouseLook(int mouseX, int mouseY);

private:

	//// Systems & engine ////
    sge::Spade* engine;
    sge::RenderSystem* renderer;
	sge::PhysicsSystem* physicsSystem;
	sge::EntityManager entityManager;
	////
	//// Other values ////
	float alpha;
	float cubeX, cubeY, cubeZ;
	////

	//// Entities ////
    std::vector<sge::Entity*> cameras;
	std::vector<sge::Entity*> modelEntities;
	std::vector<sge::Entity*> pointLights;
    sge::Entity* camera;
	sge::Entity* largeCube;
	sge::Entity* room;

	// Point lights
	sge::Entity* pointLight1;
	sge::Entity* pointLight2;
	sge::Entity* pointLight3;
	////

	//// MouseLook variables ////
	bool useMouse;
	float lastX, lastY;
	float yaw, pitch;
	int mouseXpos, mouseYpos;
	int mousseX, mousseY;
	bool firstMouse = true;
	float camSpeed;
	////

	//// Rendering stuff ////
	sge::math::mat4 V;
	sge::math::mat4 P;

	sge::Pipeline* pipelineNormals;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader2;
	sge::Shader* pixelShader2;
	sge::Texture* texture;
	sge::Texture* texture2;
	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	UniformData2 uniformData2;
	////

	//// Camera ////
	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	////
	
	//// Model handles ////
	sge::Handle<sge::ModelResource> cubeModelHandle;
	sge::Handle<sge::ModelResource> roomModelHandle;
	////

	//// Collision shapes ////
	btCollisionShape* boxShape;
	////

	//// Component factories ////
	sge::ComponentFactory<sge::TransformComponent> transformFactory;
	sge::ComponentFactory<sge::SpriteComponent> spriteFactory;
	sge::ComponentFactory<sge::CameraComponent> cameraFactory;
	sge::ComponentFactory<sge::TextComponent> textFactory;
	sge::ComponentFactory<sge::ModelComponent> modelFactory;
	sge::ComponentFactory<sge::PointLightComponent> pLightFactory;
	////

	
};