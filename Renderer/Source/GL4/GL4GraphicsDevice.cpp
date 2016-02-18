#ifdef OPENGL4

#include <iostream>

#include "SDL/SDL.h"
#include "glad/glad.h"

#include "Renderer/GraphicsDevice.h"
#include "Renderer/GL4/GL4Buffer.h"
#include "Renderer/GL4/GL4Pipeline.h"
#include "Renderer/GL4/GL4Shader.h"
#include "Renderer/GL4/GL4Texture.h"
#include "Renderer/Viewport.h"
#include "Renderer/Window.h"

#include "Core/Assert.h"

namespace sge
{
	struct GraphicsDevice::Impl
	{
		Impl(Window& window) :
			window(window.getSDLWindow()), context(SDL_GL_CreateContext(window.getSDLWindow())), pipeline(nullptr)
		{
		}

		~Impl()
		{
			SDL_GL_DeleteContext(context);
		}

		SDL_Window* window;
		SDL_GLContext context;
		GL4Pipeline* pipeline;
	};

	GraphicsDevice::GraphicsDevice(Window& window) :
		impl(new Impl(window))
	{
	}

	GraphicsDevice::~GraphicsDevice()
	{
		delete impl;
	}

	void GraphicsDevice::init()
	{
		if (!gladLoadGL())
		{
			// TODO: Debug log
		}

		int major, minor;

		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		std::cout << "RUNNING OPENGL " << major << "." << minor << std::endl;

		int max;

		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);

		std::cout << "MAX UNIFORM BUFFER BINDINGS: " << max << std::endl;

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

		glEnable(GL_MULTISAMPLE);		
	}

	void GraphicsDevice::deinit()
	{

	}

	void GraphicsDevice::swap()
	{
		SDL_GL_SwapWindow(impl->window);
	}

	void GraphicsDevice::clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	Buffer* GraphicsDevice::createBuffer(BufferType type, BufferUsage usage, size_t size)
	{
		GL4Buffer* buffer = new GL4Buffer();
		
		glGenBuffers(1, &buffer->id);

		switch (type)
		{
		case BufferType::INDEX: buffer->target = GL_ELEMENT_ARRAY_BUFFER; break;
		case BufferType::VERTEX: buffer->target = GL_ARRAY_BUFFER; break;
		case BufferType::UNIFORM: buffer->target = GL_UNIFORM_BUFFER; break;
		}

		switch (usage)
		{
		case BufferUsage::DYNAMIC: buffer->usage = GL_DYNAMIC_DRAW; break;
		case BufferUsage::STATIC: buffer->usage = GL_STATIC_DRAW; break;
		}

		return &buffer->header;
	}

	void GraphicsDevice::deleteBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);

		glDeleteBuffers(1, &gl4Buffer->id);
		delete gl4Buffer;
		buffer = nullptr;
	}

	Pipeline* GraphicsDevice::createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader)
	{
		GL4Pipeline* gl4Pipeline = new GL4Pipeline();
		GL4Shader* gl4VertexShader = reinterpret_cast<GL4Shader*>(vertexShader);
		GL4Shader* gl4PixelShader = reinterpret_cast<GL4Shader*>(pixelShader);

		SGE_ASSERT(glGetError() == GL_NO_ERROR);

		glGenVertexArrays(1, &gl4Pipeline->vao);
		glBindVertexArray(gl4Pipeline->vao);

		GLint success;
		GLchar infoLog[512];

		gl4Pipeline->vertexLayout.count = vertexLayoutDescription->count;

		size_t stride = 0;

		for (size_t i = 0; i < gl4Pipeline->vertexLayout.count; i++)
		{
			gl4Pipeline->vertexLayout.elements[i].size = vertexLayoutDescription->elements[i].size;
			gl4Pipeline->vertexLayout.elements[i].offset = stride;
			stride += vertexLayoutDescription->elements[i].size;
		}

		gl4Pipeline->vertexLayout.stride = stride;

		gl4Pipeline->program = glCreateProgram();

		SGE_ASSERT(glGetError() == GL_NO_ERROR);

		glAttachShader(gl4Pipeline->program, gl4VertexShader->id);
		glAttachShader(gl4Pipeline->program, gl4PixelShader->id);
		glLinkProgram(gl4Pipeline->program);

		glGetProgramiv(gl4Pipeline->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(gl4Pipeline->program, 512, nullptr, infoLog);
			std::cout << "ERROR: Program linking: " << std::endl << infoLog << std::endl;
		}
		else
		{
			std::cout << "ERROR: No errors :D in program linking" << std::endl;
		}

		GLint numberOfUniformBlocks = 0;

		SGE_ASSERT(glGetError() == GL_NO_ERROR);

		glGetProgramInterfaceiv(gl4Pipeline->program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numberOfUniformBlocks);

		SGE_ASSERT(glGetError() == GL_NO_ERROR);

		std::cout << "Active uniform blocks: " << numberOfUniformBlocks << std::endl;
		
		glBindVertexArray(0);

		return &gl4Pipeline->header;
	}

	void GraphicsDevice::deletePipeline(Pipeline* pipeline)
	{
		GL4Pipeline* gl4Pipeline = reinterpret_cast<GL4Pipeline*>(pipeline);
		glDeleteProgram(gl4Pipeline->program);
		glDeleteVertexArrays(1, &gl4Pipeline->vao);

		delete gl4Pipeline;
		pipeline = nullptr;
	}

	Shader* GraphicsDevice::createShader(ShaderType type, const char* source, size_t size)
	{
		GL4Shader* shader = new GL4Shader();
		GLint success;
		GLchar infoLog[512];

		switch (type)
		{
		case ShaderType::VERTEX: shader->id = glCreateShader(GL_VERTEX_SHADER); break;
		case ShaderType::PIXEL: shader->id = glCreateShader(GL_FRAGMENT_SHADER); break;
		}

		glShaderSource(shader->id, 1, &source, nullptr);
		glCompileShader(shader->id);

		glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader->id, 512, nullptr, infoLog);
			std::cout << "ERROR: Shader compilation: " << std::endl << infoLog << std::endl;
		}
		else
		{
			std::cout << "ERROR: No errors "":D""" << std::endl;
		}

		return &shader->header;
	}

	void GraphicsDevice::deleteShader(Shader* shader)
	{
		GL4Shader* gl4Shader = reinterpret_cast<GL4Shader*>(shader);
		glDeleteShader(gl4Shader->id);
		delete gl4Shader;
		shader = nullptr;
	}

	Texture* GraphicsDevice::createTexture(size_t width, size_t height, unsigned char* source)
	{
		GL4Texture* gl4Texture = new GL4Texture();

   		glGenTextures(1, &gl4Texture->id);
		glBindTexture(GL_TEXTURE_2D, gl4Texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);

		glBindTexture(GL_TEXTURE_2D, 0);

		return &gl4Texture->header;
	}

	void GraphicsDevice::deleteTexture(Texture* texture)
	{
		GL4Texture* gl4Texture = reinterpret_cast<GL4Texture*>(texture);
		glDeleteTextures(1, &gl4Texture->id);
		delete gl4Texture;
		texture = nullptr;
	}

	void GraphicsDevice::bindPipeline(Pipeline* pipeline)
	{
		GL4Pipeline* gl4Pipeline = reinterpret_cast<GL4Pipeline*>(pipeline);
		glUseProgram(gl4Pipeline->program);
		glBindVertexArray(gl4Pipeline->vao);

		impl->pipeline = gl4Pipeline;
	}

	void GraphicsDevice::debindPipeline(Pipeline* pipeline)
	{
		glUseProgram(0);
		glBindVertexArray(0);

		impl->pipeline = nullptr;
	}

	void bindBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBindBuffer(gl4Buffer->target, gl4Buffer->id);
	}

	void GraphicsDevice::bindVertexBuffer(Buffer* buffer)
	{
		SGE_ASSERT(impl->pipeline);

		bindBuffer(buffer);

		for (size_t i = 0; i < impl->pipeline->vertexLayout.count; i++)
		{
			glEnableVertexAttribArray(i);

			glVertexAttribPointer(
				i, 
				impl->pipeline->vertexLayout.elements[i].size, 
				GL_FLOAT, 
				GL_FALSE, 
				impl->pipeline->vertexLayout.stride * sizeof(GLfloat), 
				(void*)(impl->pipeline->vertexLayout.elements[i].offset * sizeof(GLfloat)));
		}
	}

	void GraphicsDevice::bindIndexBuffer(Buffer* buffer)
	{
		SGE_ASSERT(impl->pipeline);

		bindBuffer(buffer);
	}

	void GraphicsDevice::bindVertexUniformBuffer(Buffer* buffer, size_t slot)
	{
		SGE_ASSERT(impl->pipeline);

		bindBuffer(buffer);

		glBindBufferBase(GL_UNIFORM_BUFFER, slot, reinterpret_cast<GL4Buffer*>(buffer)->id);
	}

	void GraphicsDevice::bindPixelUniformBuffer(Buffer* buffer, size_t slot)
	{
		SGE_ASSERT(impl->pipeline);

		bindBuffer(buffer);

		glBindBufferBase(GL_UNIFORM_BUFFER, slot, reinterpret_cast<GL4Buffer*>(buffer)->id);
	}

	void GraphicsDevice::bindViewport(Viewport* viewport)
	{
		glViewport(viewport->x, viewport->y, viewport->width, viewport->height);
	}

	void GraphicsDevice::bindTexture(Texture* texture, size_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GL4Texture*>(texture)->id);
	}

	void GraphicsDevice::debindTexture(Texture* texture)
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GraphicsDevice::copyData(Buffer* buffer, size_t size, const void* data)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBufferData(gl4Buffer->target, size, data, gl4Buffer->usage);
		gl4Buffer->header.size = size;
	}

	void GraphicsDevice::copySubData(Buffer* buffer, size_t offset, size_t size, const void* data)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBufferSubData(gl4Buffer->target, offset, size, data);
	}

	void GraphicsDevice::draw(size_t count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void GraphicsDevice::drawIndexed(size_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void GraphicsDevice::drawInstanced(size_t count, size_t instanceCount)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, count, instanceCount);
	}

	void GraphicsDevice::drawInstancedIndexed(size_t count, size_t instanceCount)
	{
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
	}
}

#endif