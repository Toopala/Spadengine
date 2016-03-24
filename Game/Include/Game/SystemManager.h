#pragma once
#include <unordered_map>
#include "Game/System.h"
#include "Game/Component.h"
#include "Game/TestSystem.h"
#include "Game/TestComponent.h"
#include "Core/Memory/PagePoolAllocator.h"

namespace sge
{

	class SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		using Systems = std::unordered_map<size_t, System*>;

		void init();
		void addToSystem(Component* comp);
		void updateSystems();

	private:
		Systems systems;
		TestSystem* testSys;
	
	};
}
