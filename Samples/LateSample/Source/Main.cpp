#include "Spade/Spade.h"
#include "Scene.h"

int main(int argc, char** argv)
{
	sge::Spade spade;
	spade.init();
	spade.run(new Scene(&spade));
	spade.quit();

	return 0;
}