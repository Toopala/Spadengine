#include "Spade/Spade.h"
#include "TestScene.h"
#include "BulletTestScene.h"

int main(int argc, char** argv)
{
	sge::Spade engine;

	engine.init();
	engine.run(new BulletTestScene(&engine));
	engine.quit();
	return 0;
};