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

    sge::Entity* createEarth();
    sge::Entity* createPerspectiveCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createOrthoCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createSun();
    sge::Entity* createSkyBox();
    sge::Entity* createSprite(int x, int y, unsigned int width, unsigned int height, sge::Texture* texture);

    sge::Entity* earth;
    sge::Entity* defaultCamera;
    sge::Entity* fullscreenCamera;
    sge::Entity* sun;
    sge::Entity* skybox;
    sge::Entity* screen1;

    sge::RenderTarget* renderTarget;

    sge::Spade* engine;
    sge::RenderSystem* renderer;
    sge::GraphicsDevice* device;

    sge::Handle<sge::ModelResource> earthResource;
    sge::Handle<sge::ModelResource> sunResource;
    sge::Handle<sge::ModelResource> skyBoxResource;

    sge::Pipeline* pipeline;
    sge::Pipeline* skyBoxPipeline;
    sge::Pipeline* noLightsPipeline;

    sge::Shader* vertexShader;
    sge::Shader* pixelShader;
    sge::Shader* skyBoxVertexShader;
    sge::Shader* skyBoxPixelShader;
    sge::Shader* noLightsVertexShader;
    sge::Shader* noLightsPixelShader;

    sge::CubeMap* skyBoxCubeMap;

    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::ModelComponent> modelFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
    sge::ComponentFactory<sge::DirLightComponent> dirLightFactory;
    sge::ComponentFactory<sge::PointLightComponent> pointLightFactory;
    sge::ComponentFactory<sge::SpriteComponent> spriteFactory;

    sge::EntityManager entityManager;
};