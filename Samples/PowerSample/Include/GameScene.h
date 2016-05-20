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
    struct Shader;
    struct Texture;

    class DirLightComponent;
    class PointLightComponent;
    class SpotLightComponent;
    class ModelComponent;
    class TransformComponent;
    class CameraComponent;
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
    sge::Entity* createCamera(int x, int y, unsigned int width, unsigned int height);
    sge::Entity* createSun();
    sge::Entity* createSkyBox();

    sge::Entity* earthEntity;
    sge::Entity* cameraEntity;
    sge::Entity* sunEntity;
    sge::Entity* skyBoxEntity;

    sge::Spade* engine;
    sge::RenderSystem* renderer;
    sge::GraphicsDevice* device;

    sge::Handle<sge::ModelResource> earthResource;
    sge::Handle<sge::ModelResource> sunResource;
    sge::Handle<sge::ModelResource> skyBoxResource;

    sge::Pipeline* pipeline;
    sge::Pipeline* skyBoxPipeline;

    sge::Shader* vertexShader;
    sge::Shader* pixelShader;
    sge::Shader* skyBoxVertexShader;
    sge::Shader* skyBoxPixelShader;

    sge::CubeMap* skyBoxCubeMap;

    sge::ComponentFactory<sge::TransformComponent> transformFactory;
    sge::ComponentFactory<sge::ModelComponent> modelFactory;
    sge::ComponentFactory<sge::CameraComponent> cameraFactory;
    sge::ComponentFactory<sge::DirLightComponent> dirLightFactory;
    sge::ComponentFactory<sge::PointLightComponent> pointLightFactory;

    sge::EntityManager entityManager;
};