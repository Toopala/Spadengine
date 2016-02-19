#ifdef OPENGL4

#include <iostream>
#include <string>


#include "SDL2/SDL.h"

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
	void checkError()
	{
		GLenum err = GL_NO_ERROR;

		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::string error;

			switch (err)
			{
			case GL_INVALID_OPERATION: error = "INVALID OPERATION"; break;
			case GL_INVALID_ENUM: error = "INVALID ENUM"; break;
			case GL_INVALID_VALUE: error = "INVALID VALUE"; break;
			case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID FRAMEBUFFER OPERATION"; break;
			}

			std::cout << "GL ERROR: (" << err << ") " << error << std::endl;
		}
	}

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
			// TODO Debug log
		}

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_MULTISAMPLE);

		checkError();
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

		checkError();
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

		checkError();

		return &buffer->header;
	}

	void GraphicsDevice::deleteBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);

		glDeleteBuffers(1, &gl4Buffer->id);

		checkError();

		delete gl4Buffer;
		buffer = nullptr;
	}

	Pipeline* GraphicsDevice::createPipeline(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader, Shader* pixelShader)
	{
		GL4Pipeline* gl4Pipeline = new GL4Pipeline();
		GL4Shader* gl4VertexShader = reinterpret_cast<GL4Shader*>(vertexShader);
		GL4Shader* gl4PixelShader = reinterpret_cast<GL4Shader*>(pixelShader);

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

		glAttachShader(gl4Pipeline->program, gl4VertexShader->id);
		glAttachShader(gl4Pipeline->program, gl4PixelShader->id);
		glLinkProgram(gl4Pipeline->program);

		glGetProgramiv(gl4Pipeline->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(gl4Pipeline->program, 512, nullptr, infoLog);
			std::cout << "GL ERROR: Program linking: " << std::endl << infoLog << std::endl;

			glDeleteProgram(gl4Pipeline->program);
		}

		glBindVertexArray(0);

		checkError();

		return &gl4Pipeline->header;
	}

	void GraphicsDevice::deletePipeline(Pipeline* pipeline)
	{
		GL4Pipeline* gl4Pipeline = reinterpret_cast<GL4Pipeline*>(pipeline);
		glDeleteProgram(gl4Pipeline->program);
		glDeleteVertexArrays(1, &gl4Pipeline->vao);

		checkError();

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
			std::cout << "GL ERROR: Shader compilation: " << std::endl << infoLog << std::endl;
		}

		checkError();

		return &shader->header;
	}

	void GraphicsDevice::deleteShader(Shader* shader)
	{
		GL4Shader* gl4Shader = reinterpret_cast<GL4Shader*>(shader);
		glDeleteShader(gl4Shader->id);

		checkError();

		delete gl4Shader;
		shader = nullptr;
	}

	Texture* GraphicsDevice::createTexture(size_t width, size_t height, unsigned char* source)
	{
		GL4Texture* gl4Texture = new GL4Texture();

   		glGenTextures(1, &gl4Texture->id);
		glBindTexture(GL_TEXTURE_2D, gl4Texture->id);

		// TODO testing anisotropic filtering
		// TODO check for anisotropy support
		float maxValue;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxValue);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxValue);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		checkError();

		return &gl4Texture->header;
	}

	void GraphicsDevice::deleteTexture(Texture* texture)
	{
		GL4Texture* gl4Texture = reinterpret_cast<GL4Texture*>(texture);
		glDeleteTextures(1, &gl4Texture->id);

		checkError();

		delete gl4Texture;
		texture = nullptr;
	}

	void GraphicsDevice::bindPipeline(Pipeline* pipeline)
	{
		GL4Pipeline* gl4Pipeline = reinterpret_cast<GL4Pipeline*>(pipeline);
		glUseProgram(gl4Pipeline->program);
		glBindVertexArray(gl4Pipeline->vao);

		checkError();

		impl->pipeline = gl4Pipeline;
	}

	void GraphicsDevice::debindPipeline(Pipeline* pipeline)
	{
		glUseProgram(0);
		glBindVertexArray(0);

		checkError();

		impl->pipeline = nullptr;
	}

	void bindBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBindBuffer(gl4Buffer->target, gl4Buffer->id);

		checkError();
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

		checkError();
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

		checkError();
	}

	void GraphicsDevice::bindPixelUniformBuffer(Buffer* buffer, size_t slot)
	{
		SGE_ASSERT(impl->pipeline);

		bindBuffer(buffer);

		glBindBufferBase(GL_UNIFORM_BUFFER, slot, reinterpret_cast<GL4Buffer*>(buffer)->id);

		checkError();
	}

	void GraphicsDevice::bindViewport(Viewport* viewport)
	{
		glViewport(viewport->x, viewport->y, viewport->width, viewport->height);

		checkError();
	}

	void GraphicsDevice::bindTexture(Texture* texture, size_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GL4Texture*>(texture)->id);

		checkError();
	}

	void GraphicsDevice::debindTexture(Texture* texture)
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		checkError();
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

		checkError();
	}

	void GraphicsDevice::draw(size_t count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);

		checkError();
	}

	void GraphicsDevice::drawIndexed(size_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		checkError();
	}

	void GraphicsDevice::drawInstanced(size_t count, size_t instanceCount)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, count, instanceCount);

		checkError();
	}

	void GraphicsDevice::drawInstancedIndexed(size_t count, size_t instanceCount)
	{
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);

		checkError();
	}
}

#endif