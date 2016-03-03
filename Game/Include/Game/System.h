#pragma once
#include "Game/Component.h"

namespace sge
{
	class System
	{
	public:
		System();
		virtual ~System();
		virtual void addComponent(Component* comp) = 0;
	};
}
