#include "Game/EntityManager.h"
#include "Game/TransformComponent.h"

int main(int argc, char** argv)
{
	sge::EntityManager* EManager = new sge::EntityManager();

	sge::Entity* player = EManager->createEntity();

	EManager->setComponent(player, new sge::TransformComponent(player));
	sge::TransformComponent* comp = player->getComponent<sge::TransformComponent>();

	EManager->removeComponent<sge::TransformComponent>(*player);

	std::getchar();

	return 0;
}