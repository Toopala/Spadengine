#pragma once

#include "Renderer/Enumerations.h"

namespace sge
{
	class Window;
	struct Buffer;
	struct Pipeline;
	struct Shader;
	struct Texture;
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

		Pipeline* createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader);
		void deletePipeline(Pipeline* pipeline);

		Shader* createShader(ShaderType type, const char* source);
		void deleteShader(Shader* shader);

		Texture* createTexture(size_t width, size_t height, unsigned char* source);
		void deleteTexture(Texture* texture);

		void bindPipeline(Pipeline* pipeline);
		void debindPipeline(Pipeline* pipeline);

		void bindVertexBuffer(Buffer* buffer);
		void bindIndexBuffer(Buffer* buffer);
		void bindVertexUniformBuffer(Buffer* buffer, size_t slot);
		void bindPixelUniformBuffer(Buffer* buffer, size_t slot);

		void bindViewport(Viewport* viewport);

		void bindTexture(Texture* texture, size_t slot);
		void debindTexture(Texture* texture);

		void copyData(Buffer* buffer, size_t size, const void* data);
		void copySubData(Buffer* buffer, size_t offset, size_t size, const void* data);

		void draw(size_t count);
		void drawIndexed(size_t count);
		
	private:
		struct Impl;
		Impl* impl;
	};
}
