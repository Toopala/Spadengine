#pragma once

#include "Renderer/Viewport.h"
#include "Spade/Spade.h"
#include "Game/Scene.h"
#include "Core/Math.h"
#include "Resources/ModelResource.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "Game/EntityManager.h"
#include "Game/SystemManager.h"
#include "Game/CameraComponent.h"
#include "Game/ModelComponent.h"
#include "Game/TransformComponent.h"

class VisualScene : public sge::Scene
{
public:
	VisualScene(sge::Spade* engine);
	~VisualScene();

	void update(float step);
	void interpolate(float alpha);
	void draw();

private:
	sge::Spade *engine;
	sge::RenderSystem *renderer;
	
};