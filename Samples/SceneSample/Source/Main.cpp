#include "Spade/Spade.h"
#include "TestScene.h"
#include "BulletTestScene.h"

int main(int argc, char** argv)
{
	sge::Spade spade;
	spade.init();
	spade.run(new BulletTestScene(&spade));
	spade.quit();

	return 0;
};