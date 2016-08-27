#include "Game/Scene.h"

#include "Game/ComponentFactory.h"
#include "Game/EntityManager.h"

#include "Resources/ModelResource.h"
#include "Resources/ResourceManager.h"

// Forward declares
namespace sge
{
    class Entity;
    class RenderSystem;
    class Spade;

    struct CubeMap;
    struct Pipeline;
    struct RenderTarget;
    struct Shader;
    struct Texture;

    class DirLightComponent;
    class PointLightComponent;
    class SpotLightComponent;
    class ModelComponent;
    class TransformComponent;
    class CameraComponent;
    class SpriteComponent;
}

class GameScene : public sge::Scene
{
public:
    GameScene(sge::Spade* engine);
    ~GameScene();

    void update(float step);
    void interpolate(float alpha);
    void draw();

private:
    void initPipelines();
    void initResources();

    void renderScene();
    void renderFullscreen();

    sge::Entity* createEarth();
    sge::Entity* createPerspectiveCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createOrthoCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createSun();
    sge::Entity* createSkyBox();
    sge::Entity* createSprite(int x, int y, unsigned int width, unsigned int height, sge::Texture* texture);
	sge::Entity* createSpaceShip();
	sge::Entity* createMoon();

    sge::Entity* earth;
    sge::Entity* overviewCamera;
    sge::Entity* fullscreenCamera;
	sge::Entity* spaceShipCamera;
	sge::Entity* earthCamera;
    sge::Entity* sun;
    sge::Entity* skybox;
	sge::Entity* spaceShip;
	sge::Entity* moon;
	sge::Entity* fullScreen;
	sge::Entity* overviewScreen;
	sge::Entity* earthScreen;
	sge::Entity* spaceShipScreen;

	sge::RenderTarget* fullScreenTarget;
    sge::RenderTarget* overviewScreenTarget;
	sge::RenderTarget* earthScreenTarget;
	sge::RenderTarget* spaceShipScreenTarget;

    sge::Spade* engine;
    sge::RenderSystem* renderer;
    sge::GraphicsDevice* device;

    sge::Handle<sge::ModelResource> earthResource;
    sge::Handle<sge::ModelResource> sunResource;
    sge::Handle<sge::ModelResource> skyBoxResource;
	sge::Handle<sge::ModelResource> spaceShipResource;
	sge::Handle<sge::ModelResource> moonResource;

    sge::Pipeline* pipeline;
    sge::Pipeline* skyBoxPipeline;
    sge::Pipeline* noLightsPipeline;
    sge::Pipeline* bloomPipeline;

    sge::Shader* vertexShader;
    sge::Shader* pixelShader;
    sge::Shader* skyBoxVertexShader;
    sge::Shader* skyBoxPixelShader;
    sge::Shader* noLightsVertexShader;
    sge::Shader* noLightsPixelShader;
    sge::Shader* bloomVertexShader;
    sge::Shader* bloomPixelShader;

    sge::CubeMap* skyBoxCubeMap;

    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::ModelComponent> modelFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
    sge::ComponentFactory<sge::DirLightComponent> dirLightFactory;
    sge::ComponentFactory<sge::PointLightComponent> pointLightFactory;
    sge::ComponentFactory<sge::SpriteComponent> spriteFactory;

    sge::EntityManager entityManager;
};