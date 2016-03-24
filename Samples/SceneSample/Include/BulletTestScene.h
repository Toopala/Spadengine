#pragma once

#include "Renderer/Viewport.h"
#include "Spade/Spade.h"
#include "Game/Scene.h"
#include "Core/Math.h"
#include "Resources/ModelResource.h"

class BulletTestScene : public sge::Scene
{
public:
	BulletTestScene(sge::Spade* engine);
	~BulletTestScene();

	void update(float step);
	void interpolate(float alpha);
	void draw();
private:
	sge::Spade* engine;
	sge::Viewport viewport;
};
