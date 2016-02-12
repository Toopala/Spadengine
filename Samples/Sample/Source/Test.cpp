#include "Core/Math.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "SDL/SDL.h"
#include "Game/EntityManager.h"
#include "Core/Memory/PagePoolAllocator.h"


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <iostream>

#include "Model.h"

class MemoryTest
{
public:
	MemoryTest(int a, int b, int c, std::string d)
	{
		testia = a;
		testib = b;
		testic = c;
		testid = d;
		std::cout << "Constructed" << std::endl;
	}
	~MemoryTest()
	{
		std::cout << "Destroyed!" << std::endl;
	}
	int testia, testib, testic;
	std::string testid;
};

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	sge::math::vec2 vec;
	std::cout << vec.x << ", " << vec.y << ", " << sge::math::haeSata() << std::endl;

	sge::Window window("Spade Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	sge::GraphicsDevice device(window);

	device.init();

	int w, h, n;

	unsigned char* data = stbi_load("rockwall_diffuse_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_diffuse_map.png: " << w << "x" << h << " and something like " << n << std::endl;

	unsigned char* data2 = stbi_load("rockwall_normal_map.png", &w, &h, &n, STBI_rgb_alpha);

	std::cout << "Opened image rockwall_normal_map.png: " << w << "x" << h << " and something like " << n << std::endl;




	const char* VERTEX_SOURCE =
		"#version 440\n"

		"in vec3 inPosition;\n"
		"in vec3 inNormal;\n"
		"in vec3 inTangent;\n"
		"in vec3 inBitangent;\n"
		"in vec2 inTexcoords;\n"

		"out vec2 texcoords;\n"
		"out vec3 normals;\n"

		"out vec3 TangentFragPos;\n"
		"out vec3 TangentLightPos;\n"

		"layout (std140, binding = 0) uniform MVPUniform\n"
		"{\n"
		"mat4 MVP;\n"
		"mat4 M;\n"
		"};\n"

		"void main()\n"
		"{\n"
		"	gl_Position = MVP * vec4(inPosition, 1.0);\n"
		"	vec3 FragPos = vec3(M * vec4(inPosition, 1.0));\n"
		"	texcoords = inTexcoords;\n"
		"	mat3 normalMatrix = transpose(inverse(mat3(M)));\n"
		"	vec3 T = normalize(normalMatrix * inTangent);\n"
		"	vec3 B = normalize(normalMatrix * inBitangent);	\n"
		"	vec3 N = normalize(normalMatrix * inNormal);		\n"
		"	mat3 TBN = transpose(mat3(T, B, N));	\n"
		"	TangentFragPos = TBN * FragPos;			\n"
		"	vec3 L = vec3(2.0, 3.0, 3.0); \n"
		"	TangentLightPos = TBN * L;				\n"
		"	normals = inNormal;\n"
		"}\n";

	const char* PIXEL_SOURCE =
		"#version 440\n"

		"in vec2 texcoords;\n"
		"in vec3 normals;\n"

		"in vec3 TangentLightPos;  \n"
		"in vec3 TangentFragPos;   \n"

		"out vec4 outColour;\n"

		"layout(binding = 0) uniform sampler2D tex;\n"
		"layout(binding = 1) uniform sampler2D tex2;\n"

		"void main()\n"
		"{\n"
		"	vec3 normal = normalize(normals);															\n"
		"	normal = texture(tex2, texcoords).rgb;													\n"
		"	normal = normalize(normal * 2.0 - 1.0);													\n"
		"																								\n"
		"	vec3 color = texture(tex, texcoords).rgb;											\n"
		"	vec3 ambient = 0.2*color;																	\n"
		"																								\n"
		"	vec3 ligthDirection = TangentLightPos - TangentFragPos;									\n"
		"																								\n"
		"	float distance = length(TangentLightPos - TangentFragPos);									\n"
		"	float attenuation = 1.0 / (1.0 + 0.0000009 * distance + 0.0016 * (distance * distance));	\n"
		"																								\n"
		"	float diff = max(0.0, dot(normalize(normal), normalize(ligthDirection)));					\n"
		"	vec3 diffuse = diff * color;																\n"
		"	outColour = vec4(diffuse*attenuation + ambient*attenuation, 1.0);			   \n"
		"}\n";

	float width = 0.1f;
	float height = 0.1f;
	float alpha = 0.0f;

	float vertexData[] =
	{
		width, 2 * height, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
		2 * width, 2 * height, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		2 * width, height, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		width, height, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	};

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 P = glm::perspective(glm::radians(66.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 VP = P*V;

	glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glm::mat4 MVP = VP*M;

	sge::math::mat4 uniformData[] =
	{
		MVP,
		M
	};

	//Assimp test
	Model* model = new Model("cube.dae");

	std::vector<Vertex>* vertices = model->getVerticeArray();
	std::vector<unsigned int>* indices = model->getIndexArray();

	unsigned int indexData[] =
	{
		0, 1, 2, 0, 2, 3
	};

	sge::VertexLayoutDescription vertexLayoutDescription = { 5, { 3, 3, 3, 3, 2 } };

	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, VERTEX_SOURCE);
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, PIXEL_SOURCE);
	sge::Texture* texture = device.createTexture(w, h, data);
	sge::Texture* texture2 = device.createTexture(w, h, data2);

	stbi_image_free(data);
	stbi_image_free(data2);

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::STATIC);
	sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::STATIC);
	sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::STATIC);

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.bindIndexBuffer(indexBuffer);
	device.bindVertexUniformBuffer(uniformBuffer, 0);
	device.bindTexture(texture, 0);
	device.bindTexture(texture2, 1);

	device.copyData(vertexBuffer, vertices->size() * sizeof(Vertex), vertices->data());
	//device.copyData(vertexBuffer, sizeof(vertexData), vertexData);
	//device.copyData(indexBuffer, sizeof(indexData), indexData);
	device.copyData(uniformBuffer, sizeof(uniformData), uniformData);

	SDL_Event event;

	bool running = true;

	float temp = 0;

	
	
	// Memory allocation test

	MemoryTest *mt1 = sge::allocator.create<MemoryTest>(10, 10, 10, "10");
	sge::allocator.destroy<MemoryTest>(mt1);

	MemoryTest *mt5 = sge::allocator.create<MemoryTest>(1,2,3,"4");
	sge::allocator.destroy<MemoryTest>(mt5);

	while (running)
	{
		device.clear(0.5f, 0.0f, 0.5f, 1.0f);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		M = sge::math::rotate(sge::math::mat4(), alpha, glm::vec3(1.0f, 0.2f, 0.1f));
		
		// Order must not be changed.
		MVP = VP*M;

		uniformData[0] = MVP;
		uniformData[1] = M;

		device.copySubData(uniformBuffer, 0, sizeof(uniformData), uniformData);

		device.draw(vertices->size());

		device.swap();

		alpha += 0.01;
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(indexBuffer);
	device.deleteBuffer(vertexBuffer);
	device.deleteBuffer(uniformBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);
	device.deleteTexture(texture);

	device.deletePipeline(pipeline);

	device.deinit();

	SDL_Quit();

	return 0;
}