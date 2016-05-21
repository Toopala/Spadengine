#pragma once

#include <cstddef>

#include "Renderer/Enumerations.h"

namespace sge
{
    class TextureResource;
	class Window;

	struct CubeMap;
	struct Buffer;
	struct Pipeline;
    struct RenderTarget;
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

		void swap();

		Buffer* createBuffer(BufferType type, BufferUsage usage, size_t size);
		void deleteBuffer(Buffer* buffer);

		Pipeline* createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader);
		void deletePipeline(Pipeline* pipeline);

        RenderTarget* createRenderTarget(size_t count, size_t width, size_t height, bool depth = true);
        void deleteRenderTarget(RenderTarget* renderTarget);

		Shader* createShader(ShaderType type, const char* source, size_t size);
		void deleteShader(Shader* shader);

		Texture* createTexture(TextureResource* source);
        Texture* createTextTexture(TextureResource* source);

        Texture* createTexture(size_t width, size_t height, unsigned char* source = 0);
        Texture* createTextTexture(size_t width, size_t height, unsigned char* source);

		void deleteTexture(Texture* texture);

        CubeMap* createCubeMap(TextureResource* source[]);
		void deleteCubeMap(CubeMap* cubeMap);

		void bindPipeline(Pipeline* pipeline);
		void debindPipeline(Pipeline* pipeline);

        void bindRenderTarget(RenderTarget* renderTarget);
        void debindRenderTarget();

		void bindVertexBuffer(Buffer* buffer);
		void bindIndexBuffer(Buffer* buffer);
		void bindVertexUniformBuffer(Buffer* buffer, size_t slot);
		void bindPixelUniformBuffer(Buffer* buffer, size_t slot);

		void bindViewport(Viewport* viewport);

		void bindTexture(Texture* texture, size_t slot);
		void debindTexture(Texture* texture, size_t slot);

		void bindCubeMap(CubeMap* cubeMap, size_t slot);
		void debindCubeMap(CubeMap* cubeMap, size_t slot);

		void copyData(Buffer* buffer, size_t size, const void* data);
		void copySubData(Buffer* buffer, size_t offset, size_t size, const void* data);

		void draw(size_t count);
		void drawIndexed(size_t count);
		void drawInstanced(size_t count, size_t instanceCount);
		void drawInstancedIndexed(size_t count, size_t instanceCount);
		
	private:
		struct Impl;
		Impl* impl;
	};
}
