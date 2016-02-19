#include "Spade\Spade.h"
#include "TestScene.h"

int main(int argc, char** argv)
{
	sge::Spade engine;

	engine.init();
	engine.run(new sge::TestScene(&engine));
	engine.quit();
	return 0;
};