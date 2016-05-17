#include "Spade/Spade.h"
#include "VisualScene.h"

int main(int argc, char** argv)
{
	sge::Spade spade;
	spade.init();
	spade.run(new VisualScene(&spade));
	spade.quit();
	
	return 0;
}