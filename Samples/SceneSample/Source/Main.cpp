#include "Spade/Spade.h"
#include "TestScene.h"
#include "BulletTestScene.h"

int main(int argc, char** argv)
{
	sge::Spade::getInstance().init();
	sge::Spade::getInstance().run(new BulletTestScene());
	sge::Spade::getInstance().quit();

	return 0;
};