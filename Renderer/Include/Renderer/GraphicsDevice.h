#pragma once

#include "Renderer/Enumerations.h"

namespace sge
{
	class Window;
	struct Buffer;
	struct Pipeline;
	struct Shader;
	struct Texture;
	struct VertexLayout;
	struct VertexLayoutDescription;
	struct Viewport;

	class GraphicsDevice
	{
	public:
		GraphicsDevice(Window& window);
		~GraphicsDevice();

		void init();
		void deinit();

		void clear(float r, float g, float b, float a);

		Buffer* createBuffer(BufferType type, BufferUsage usage);
		void deleteBuffer(Buffer* buffer);

		Pipeline* createPipeline(Shader* vertexShader, Shader* pixelShader);
		void deletePipeline(Pipeline* pipeline);

		Shader* createShader(ShaderType type, const char* buffer);
		void deleteShader(Shader* shader);

		VertexLayout* createVertexLayout(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader);

		void bindPipeline(Pipeline* pipeline);
		void debindPipeline(Pipeline* pipeline);

		void bindVertexBuffer(Buffer* buffer);
		void bindIndexBuffer(Buffer* buffer);

		void bindViewport(Viewport* viewport);

		void bindTexture(unsigned int index, Texture* texture);
		void debindTexture(unsigned int index);

		void copyData(Buffer* buffer, const void* data, size_t size);
		void copySubData(Buffer* buffer, size_t offset, const void* data, size_t size);

		void draw(size_t count);
		void drawIndexed(size_t count);
		
	private:
		struct Impl;

		Impl* impl;
	};
}
