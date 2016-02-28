#pragma once

namespace sge
{
	class GraphicsDevice;
	class Window;

	class Renderer
	{
	public:
		Renderer(Window& window);
		~Renderer();
	private:
		GraphicsDevice* device;
	};
}