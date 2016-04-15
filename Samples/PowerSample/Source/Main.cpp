#include "Spade/Spade.h"
#include "GameScene.h"

int main(int argc, char** argv)
{
    sge::Spade spade;

    spade.init();
    spade.run(new GameScene(&spade));
    spade.quit();

    return 0;
}