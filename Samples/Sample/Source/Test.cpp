#include "Core/Math.h"
#include "Renderer/Enumerations.h"
#include "Renderer/Window.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Pipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexLayout.h"
#include "Renderer/Viewport.h"
#include "SDL/SDL.h"
#include "Game/EntityManager.h"
#include "Core\Memory\PagePoolAllocator.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <iostream>
#include <fstream>

#include "Model.h"

class MemoryTest
{
public:
	MemoryTest(int a, int b)
	{
		testia = a;
		testib = b;
	}
	~MemoryTest(){
		std::cout << testia << " " << testib << std::endl;
	}
	int testia, testib;
};

class MemoryTest2
{
public:
	MemoryTest2(){}
	~MemoryTest2(){}
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

	// DX SHADERS

	std::ifstream file;

	file.open("Assets/Shaders/VertexShader.cso", ios::in | ios::ate | ios::binary);

	std::vector<char> vShaderData;

	if (file.is_open())
	{
		std::cout << "vShaderData opened!" << std::endl;
		vShaderData.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, ios::beg);
		file.read(vShaderData.data(), vShaderData.size());
		file.close();
	}

	file.open("Assets/Shaders/PixelShader.cso", ios::in | ios::ate | ios::binary);

	std::vector<char> pShaderData;

	if (file.is_open())
	{
		std::cout << "pShaderData opened!" << std::endl;
		pShaderData.resize(static_cast<size_t>(file.tellg()));

		file.seekg(0, ios::beg);
		file.read(pShaderData.data(), pShaderData.size());
		file.close();
	}

	// OPENGL SHADERS
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

	float width = 1.0f;
	float height = 1.0f;


	float vertexData[] =
	{
		-width, height, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		width, height, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		width, -height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-width, -height, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 P = glm::perspective(glm::radians(66.0f), 1280.0f / 720.0f, 0.1f, 1000.f);
	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	struct UniformData
	{
		sge::math::mat4 PV;
		sge::math::mat4 M;
	};

	UniformData uniformData;

	uniformData.M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	uniformData.PV = P * V;

	//Assimp test
	Model* model = new Model("cube.dae");

	std::vector<Vertex>* vertices = model->getVerticeArray();
	std::vector<unsigned int>* indices = model->getIndexArray();

	unsigned int indexData[] =
	{
		0, 1, 2, 0, 2, 3
	};

	// DX11 Layout
	//sge::VertexLayoutDescription vertexLayoutDescription = { 2, { 3, 4 } };

	sge::VertexLayoutDescription vertexLayoutDescription = { 5,
	{
		{ 0, 3, sge::VertexSemantic::POSITION },
		{ 0, 3, sge::VertexSemantic::NORMAL },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 3, sge::VertexSemantic::TANGENT },
		{ 0, 2, sge::VertexSemantic::TEXCOORD }
	} };

	// DX11 Shaders
	sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, vShaderData.data(), vShaderData.size());
	sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, pShaderData.data(), pShaderData.size());

	// GL4 Shaders
	//sge::Shader* vertexShader = device.createShader(sge::ShaderType::VERTEX, VERTEX_SOURCE, sizeof(VERTEX_SOURCE));
	//sge::Shader* pixelShader = device.createShader(sge::ShaderType::PIXEL, PIXEL_SOURCE, sizeof(PIXEL_SOURCE));

	sge::Texture* texture = device.createTexture(w, h, data);
	sge::Texture* texture2 = device.createTexture(w, h, data2);

	stbi_image_free(data);
	stbi_image_free(data2);

	sge::Pipeline* pipeline = device.createPipeline(&vertexLayoutDescription, vertexShader, pixelShader);
	sge::Viewport viewport = { 0, 0, 1280, 720 };

	// DX11 Buffers
	sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(Vertex) * vertices->size());
	sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::DYNAMIC, sizeof(indexData));
	sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData));

	// GL4 Buffers
	//sge::Buffer* vertexBuffer = device.createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, sizeof(vertices->size() * sizeof(Vertex)));
	//sge::Buffer* indexBuffer = device.createBuffer(sge::BufferType::INDEX, sge::BufferUsage::DYNAMIC, sizeof(indexData));
	//sge::Buffer* uniformBuffer = device.createBuffer(sge::BufferType::UNIFORM, sge::BufferUsage::DYNAMIC, sizeof(uniformData));

	device.bindViewport(&viewport);
	device.bindPipeline(pipeline);

	device.bindVertexBuffer(vertexBuffer);
	device.bindIndexBuffer(indexBuffer);
	device.bindVertexUniformBuffer(uniformBuffer, 0);
	device.bindTexture(texture, 0);
	device.bindTexture(texture2, 1);

	//device.copyData(vertexBuffer, vertices->size() * sizeof(Vertex), vertices->data());
	device.copyData(vertexBuffer, sizeof(Vertex) * vertices->size(), vertices->data());
	device.copyData(indexBuffer, sizeof(indexData), indexData);
	device.copyData(uniformBuffer, sizeof(uniformData), &uniformData);

	SDL_Event event;

	bool running = true;

	float temp = 0;

	// Memory allocation test
	sge::PagePoolAllocator *allocator = new sge::PagePoolAllocator;

	MemoryTest *mt = (MemoryTest*)allocator->allocate(sizeof(MemoryTest));
	new (mt)MemoryTest(2, 5);
	MemoryTest *mt2 = (MemoryTest*)allocator->allocate(sizeof(MemoryTest));
	new (mt2)MemoryTest(7, 9);
	MemoryTest *mt3 = (MemoryTest*)allocator->allocate(sizeof(mt3));
	new(mt3)MemoryTest(10, 10);

	std::cout << mt->testia << ", " << mt->testib << std::endl;
	allocator->deallocate(mt);
	allocator->deallocate(mt2);
	std::cout << mt->testia << ", " << mt->testib << std::endl;
	std::cout << mt2->testia << ", " << mt2->testib << std::endl;
	std::cout << mt3->testia << ", " << mt3->testib << std::endl;
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

		uniformData.M = sge::math::rotate(uniformData.M, glm::radians(0.01f), glm::vec3(0.5f, 1.0f, 0.5f));

		device.copyData(uniformBuffer, sizeof(uniformData), &uniformData);

		device.draw(vertices->size());

		device.swap();
	}

	device.debindPipeline(pipeline);

	device.deleteBuffer(indexBuffer);
	device.deleteBuffer(vertexBuffer);
	device.deleteBuffer(uniformBuffer);

	device.deleteShader(vertexShader);
	device.deleteShader(pixelShader);
	device.deleteTexture(texture);
	device.deleteTexture(texture2);

	device.deletePipeline(pipeline);

	device.deinit();

	SDL_Quit();

	return 0;
}