#pragma once

#include "Renderer/Viewport.h"
#include "Spade/Spade.h"
#include "Game/Scene.h"
#include "Core/Math.h"
#include "Resources/ModelResource.h"

	// FORWARD DECLARE
struct sge::Pipeline;
struct sge::Buffer;
struct sge::Viewport;
struct sge::Shader;
struct sge::Texture;

struct Vertex;

struct UniformData
{
	sge::math::mat4 PV;
	sge::math::mat4 M;
};

class TestScene : public sge::Scene
{
public:
	TestScene(sge::Spade* engine);
	~TestScene();

	void update(float step);
	void interpolate(float alpha);
	void draw();

	// Copied functions
	void mouseLook(int mouseX, int mouseY);
	void loadTextShader(const std::string& path, std::vector<char>& data);
	void loadBinaryShader(const std::string& path, std::vector<char>& data);

private:
	sge::Spade* engine;
	int mouseX, mouseY;

	float alpha;
	bool useMouse;

	// used in copied functions
	float lastX, lastY;
	float yaw, pitch;
	glm::vec3 cameraFront;
	int mouseXpos, mouseYpos;
	int mousseX, mousseY;
	bool firstMouse = true;

	sge::math::mat4 V;
	sge::math::mat4 P;
	
	sge::Pipeline* pipeline;
	sge::Viewport viewport;
	sge::Buffer* vertexBuffer;
	sge::Buffer* uniformBuffer;
	sge::Shader* vertexShader;
	sge::Shader* pixelShader;
	sge::Texture* texture;
	sge::Texture* texture2;

	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	UniformData uniformData;

	sge::ModelResource* model;

};
