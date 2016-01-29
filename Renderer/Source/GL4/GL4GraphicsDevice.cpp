#ifdef OPENGL4

#include <iostream>

#include "SDL/SDL.h"
#include "glad/glad.h"

#include "Renderer/GraphicsDevice.h"
#include "Renderer/GL4/GL4Buffer.h"
#include "Renderer/GL4/GL4Pipeline.h"
#include "Renderer/GL4/GL4Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Texture.h"
#include "Renderer/Viewport.h"
#include "Renderer/Window.h"

namespace sge
{
	class GraphicsDevice::Impl
	{
	public:
		Impl(Window& window) :
			context(SDL_GL_CreateContext(window.getSDLWindow())), pipeline(nullptr)
		{
			if (!gladLoadGL())
			{
				// TODO: Debug log
			}
		}

		~Impl()
		{
			SDL_GL_DeleteContext(context);
		}

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

	}

	void GraphicsDevice::deinit()
	{

	}

	void GraphicsDevice::clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	Buffer* GraphicsDevice::createBuffer(BufferType type, BufferUsage usage)
	{
		GL4Buffer* buffer = new GL4Buffer();
		
		glGenBuffers(1, &buffer->id);

		switch (type)
		{
		case BufferType::INDEX: buffer->target = GL_ELEMENT_ARRAY_BUFFER; break;
		case BufferType::VERTEX: buffer->target = GL_ARRAY_BUFFER; break;
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

	Pipeline* GraphicsDevice::createPipeline(Shader* vertexShader, Shader* pixelShader)
	{
		GL4Pipeline* gl4Pipeline = new GL4Pipeline();
		GL4Shader* gl4VertexShader = reinterpret_cast<GL4Shader*>(vertexShader);
		GL4Shader* gl4PixelShader = reinterpret_cast<GL4Shader*>(pixelShader);

		glGenVertexArrays(1, &gl4Pipeline->vao);
		glBindVertexArray(gl4Pipeline->vao);

		GLint success;
		GLchar infoLog[512];

		gl4Pipeline->program = glCreateProgram();

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

	VertexLayout* GraphicsDevice::createVertexLayout(VertexLayoutDescription* vertexLayoutDescription, Shader* vertexShader)
	{
		GL4Shader* gl4Shader = reinterpret_cast<GL4Shader*>(vertexShader);
		VertexLayout* vertexLayout = new VertexLayout();



		return nullptr;
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

	Shader* GraphicsDevice::createShader(ShaderType type, const char* buffer)
	{
		GL4Shader* shader = new GL4Shader();
		GLint success;
		GLchar infoLog[512];

		switch (type)
		{
		case ShaderType::VERTEX: shader->id = glCreateShader(GL_VERTEX_SHADER); break;
		case ShaderType::PIXEL: shader->id = glCreateShader(GL_FRAGMENT_SHADER); break;
		}

		glShaderSource(shader->id, 1, &buffer, nullptr);
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

	void GraphicsDevice::bindVertexBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBindBuffer(gl4Buffer->target, gl4Buffer->id);

		// Set attrib pointers here! VAO needs to be bound.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
	}

	void GraphicsDevice::bindIndexBuffer(Buffer* buffer)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBindBuffer(gl4Buffer->target, gl4Buffer->id);
	}

	void GraphicsDevice::bindViewport(Viewport* viewport)
	{
		glViewport(viewport->x, viewport->y, viewport->width, viewport->height);
	}

	void GraphicsDevice::bindTexture(unsigned int index, Texture* texture)
	{

	}

	void GraphicsDevice::debindTexture(unsigned int index)
	{

	}

	void GraphicsDevice::copyData(Buffer* buffer, const void* data, size_t size)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBufferData(gl4Buffer->target, size, data, gl4Buffer->usage);
	}

	void GraphicsDevice::copySubData(Buffer* buffer, size_t offset, const void* data, size_t size)
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
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
	}
}

#endif