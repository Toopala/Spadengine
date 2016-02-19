#pragma once

#include "Game\Scene.h"
#include "Spade\Spade.h"

namespace sge
{
	class TestScene : public Scene
	{
	public:
		TestScene(Spade* engine);
		~TestScene();

		void update(float step);
		void interpolate(float alpha);
		void draw();

	private:
		Spade* engine;
	};
};