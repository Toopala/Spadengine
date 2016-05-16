#include "GameScene.h"
#include "Spade/Spade.h"

/*
TODO enko

Tekstuuriresurssille tekstuurien generointi GPU:n muistiin
SpotLightComponent do!
Instanced rendering do!
Deferred rendering do!
Render targeteille depth-puskuri!
Optimoi bulletscenea (nykii ihan vitusti)
    - Esim valojen dataa ei tarvii viedä joka objektille erikseen
    - Sorttaus takasin kuntoon jos tarviijaksaahaluaa
Tee se demo ja siihen kaikkea hauskaa (blur, hdr voe pojat efektejä)
*/

GameScene::GameScene(sge::Spade* engine) :
    engine(engine),
    renderer(engine->getRenderer())
{
}

GameScene::~GameScene()
{
}

void GameScene::update(float step)
{
    if (engine->keyboardInput->keyIsPressed(sge::KEYBOARD_ESCAPE))
    {
        // TODO weird but works?
        engine->stop();
    }
}

void GameScene::interpolate(float alpha)
{
}

void GameScene::draw()
{
    renderer->begin();
    renderer->end();

    renderer->render();
    renderer->present();
    renderer->clear();
}