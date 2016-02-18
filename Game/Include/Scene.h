#pragma once

namespace sge
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void update(float step) = 0;
		virtual void interpolate(float alpha) = 0;
		virtual void draw() = 0;
		
	};
}