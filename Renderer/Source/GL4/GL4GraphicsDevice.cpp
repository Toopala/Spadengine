#ifdef OPENGL4

#include <iostream>
#include <string>


#include "SDL2/SDL.h"

#include "glad/glad.h"

#include "Renderer/GraphicsDevice.h"
#include "Renderer/GL4/GL4Buffer.h"
#include "Renderer/GL4/GL4Pipeline.h"
#include "Renderer/GL4/GL4RenderTarget.h"
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

		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		std::cout << "Using OpenGL version " << major << "." << minor << std::endl;
		
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);

        glCullFace(GL_BACK);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


		GLint numberOfUniformBlocks = 0;
		GLuint* uniformBlocks;

		glGetProgramInterfaceiv(gl4Pipeline->program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numberOfUniformBlocks);

		uniformBlocks = new GLuint[numberOfUniformBlocks];

		for (GLint i = 0; i < numberOfUniformBlocks; i++)
		{
			const GLenum props[] = { GL_BLOCK_INDEX };
			const GLenum props2[] = { GL_BUFFER_BINDING };

			GLint index;
			GLint binding;
			GLchar name[512];
			GLsizei size;

			//glGetProgramResourceiv(gl4Pipeline->program, GL_UNIFORM, i, 1, props, 1, nullptr, &index);
			glGetProgramResourceiv(gl4Pipeline->program, GL_UNIFORM_BLOCK, i, 1, props2, 1, nullptr, &binding);
			glGetProgramResourceName(gl4Pipeline->program, GL_UNIFORM_BLOCK, i, 512, &size, name);
			index = glGetUniformBlockIndex(gl4Pipeline->program, name);
			std::cout << "Found uniform block: " << name << " at index " << index << " with binding " << binding << std::endl;

			//glUniformBlockBinding(gl4Pipeline->program, uniformBlocks[index], index);
		}

		std::cout << "Active uniform blocks: " << numberOfUniformBlocks << std::endl;

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

    RenderTarget* GraphicsDevice::createRenderTarget(size_t count, Texture** textures)
    {
        GLint maxColorAttachments = 0;
        GLint maxDrawBuf = 0;
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        
        SGE_ASSERT((GLint)count <= maxColorAttachments && (GLint)count <= maxDrawBuf);

        GL4RenderTarget* gl4RenderTarget = new GL4RenderTarget();

        gl4RenderTarget->count = count;
        gl4RenderTarget->buffers = new GLenum[count];

        glGenFramebuffers(1, &gl4RenderTarget->id);
        glBindFramebuffer(GL_FRAMEBUFFER, gl4RenderTarget->id);

        for (size_t i = 0; i < count; i++)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, reinterpret_cast<GL4Texture*>(textures[i])->id, 0);
            gl4RenderTarget->buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            checkError();

            SGE_ASSERT(false);
        }

        checkError();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return &gl4RenderTarget->header;
    }

    void GraphicsDevice::deleteRenderTarget(RenderTarget* renderTarget)
    {
        GL4RenderTarget* gl4RenderTarget = reinterpret_cast<GL4RenderTarget*>(renderTarget);

        glDeleteFramebuffers(1, &gl4RenderTarget->id);

        checkError();

        delete[] gl4RenderTarget->buffers;
        delete gl4RenderTarget;
        renderTarget = nullptr;
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
		checkError();

		glBindTexture(GL_TEXTURE_2D, gl4Texture->id);

		checkError();
		// TODO testing anisotropic filtering
		// TODO check for anisotropy support
		float maxValue;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxValue);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxValue);

		checkError();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		checkError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);

		checkError();
		glGenerateMipmap(GL_TEXTURE_2D);

		checkError();
		glBindTexture(GL_TEXTURE_2D, 0);

		checkError();

		return &gl4Texture->header;
	}

	Texture* GraphicsDevice::createTextTexture(size_t width, size_t height, unsigned char* source)
	{
		GL4Texture* gl4Texture = new GL4Texture();

		glGenTextures(1, &gl4Texture->id);
		checkError();

		glBindTexture(GL_TEXTURE_2D, gl4Texture->id);

		checkError();
		// TODO testing anisotropic filtering
		// TODO check for anisotropy support
		float maxValue;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxValue);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxValue);

		checkError();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		checkError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, source);

		checkError();
		glGenerateMipmap(GL_TEXTURE_2D);

		checkError();
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

    void GraphicsDevice::bindRenderTarget(RenderTarget* renderTarget)
    {
        GL4RenderTarget* gl4RenderTarget = reinterpret_cast<GL4RenderTarget*>(renderTarget);

        glBindFramebuffer(GL_FRAMEBUFFER, gl4RenderTarget->id);
        glDrawBuffers(gl4RenderTarget->count, gl4RenderTarget->buffers);
    }

    void GraphicsDevice::debindRenderTarget()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

		glBindBufferBase(GL_UNIFORM_BUFFER, slot, reinterpret_cast<GL4Buffer*>(buffer)->id);

		checkError();
	}

	void GraphicsDevice::bindPixelUniformBuffer(Buffer* buffer, size_t slot)
	{
		SGE_ASSERT(impl->pipeline);

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

		checkError();
		glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GL4Texture*>(texture)->id);

		checkError();
	}

	void GraphicsDevice::debindTexture(Texture* texture, size_t slot)
	{
        glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);

		checkError();
	}

	void GraphicsDevice::copyData(Buffer* buffer, size_t size, const void* data)
	{
		GL4Buffer* gl4Buffer = reinterpret_cast<GL4Buffer*>(buffer);
		glBufferData(gl4Buffer->target, size, data, gl4Buffer->usage);
		gl4Buffer->header.size = size;
		checkError();
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