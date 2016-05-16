#include <vector>

#include "Game/Scene.h"

// Forward declares
namespace sge
{
    class Spade;
    class RenderSystem;
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
    sge::Spade* engine;
    sge::RenderSystem* renderer;
};