#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

namespace sge
{
	class Window;

	class GraphicsDevice
	{
	public:
		GraphicsDevice(Window& window);
		~GraphicsDevice();

		void setClearColor(float r, float g, float b, float a);
		void clear();
	private:
		class Impl;

		Impl* impl;
	};
}

#endif