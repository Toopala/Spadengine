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
    sge::Entity* createCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createText(float x, float y, const std::string& text);

private:
    sge::Spade* engine;
    sge::RenderSystem* renderer;
	sge::PhysicsSystem* physicsSystem;

    std::vector<sge::Entity*> cameras;

    sge::Entity* camera;
    sge::Entity* guiText;
    sge::Entity* targetEntity;
	sge::Entity* largeCube;

	///MouseLook
	bool useMouse;
	float lastX, lastY;
	float yaw, pitch;
	int mouseXpos, mouseYpos;
	int mousseX, mousseY;
	bool firstMouse = true;
	float camSpeed;
	///

	
	sge::math::mat4 V;
	sge::math::mat4 P;

	sge::Pipeline* pipelineNormals;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader2;
	sge::Shader* pixelShader2;
	sge::Texture* texture;
	sge::Texture* texture2;

	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	UniformData2 uniformData2;

	sge::Handle <sge::ModelResource> modelHandle;

	btCollisionShape* boxShape;

	/*float alpha;*/
    /*sge::Handle<sge::TextureResource> textureResource;
    sge::Handle<sge::FontResource> fontResource;
    sge::Texture* texture;
    sge::Texture** targetTextures;
    sge::RenderTarget* renderTarget;
    sge::math::mat4 VP;

    size_t targetCount;*/

    sge::EntityManager entityManager;
    


    // Component factories
    // TODO maybe we should use one factory for all component types? Using it could be nicer.
    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::SpriteComponent> spriteFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
    sge::ComponentFactory<sge::TextComponent> textFactory;
	sge::ComponentFactory<sge::ModelComponent> modelFactory;

};