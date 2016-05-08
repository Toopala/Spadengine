#pragma once

#include "Resources/Resource.h"

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

#include "Resources/TextureResource.h"
#include "Core/Math.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Texture.h"
#include "Renderer/GraphicsDevice.h"

struct Vertex {
	// Position
	sge::math::vec3 Position;
	// Normal
	sge::math::vec3 Normal;
	// Position
	sge::math::vec3 Tangent;
	// Normal
	sge::math::vec3 Bitangent;
	// UV
	sge::math::vec2 UV;
};

namespace sge
{
	class Mesh {
	public:
		/*  Mesh Data  */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<sge::TextureResource> textures;

		sge::Buffer* vertexBuffer;
		sge::Buffer* indexBuffer;

		/*  Functions  */
		// Constructor
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<sge::TextureResource> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
		}

		void createBuffers(GraphicsDevice* device)
		{
			//plaa
			vertexBuffer = device->createBuffer(sge::BufferType::VERTEX, sge::BufferUsage::DYNAMIC, vertices.size()*sizeof(Vertex));
			indexBuffer = device->createBuffer(sge::BufferType::INDEX, sge::BufferUsage::DYNAMIC, indices.size()*sizeof(unsigned int));
			device->bindVertexBuffer(vertexBuffer);
			device->bindIndexBuffer(indexBuffer);
			device->copyData(vertexBuffer, sizeof(Vertex) * vertices.size(), vertices.data());
		}

		sge::Buffer* getVertexBuffer()
		{
			return vertexBuffer;
		}

		sge::Buffer* getIndexBuffer()
		{
			return indexBuffer;
		}
	};

	class ModelResource : public sge::Resource
	{
	public:
		// Constructor, expects a filepath to a 3D model.
		ModelResource(const std::string& resourcePath);
		~ModelResource();

		std::vector<Vertex>* getVerticeArray();
		std::vector<unsigned int>* getIndexArray();
		sge::Texture* getDiffuseTexture();
		sge::Texture* getNormalTexture();
		sge::Texture* getSpecularTexture();

		void createBuffers();

		sge::Buffer* getVertexBuffer();
		sge::Buffer* getIndexBuffer();

        void setDevice(GraphicsDevice* device) { this->device = device; }

	private:
        GraphicsDevice* device;
		/*  Model Data  */
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<sge::TextureResource> textures_loaded; // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

		/*  Functions   */
		// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void loadModel(std::string path);

		// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<sge::TextureResource> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}