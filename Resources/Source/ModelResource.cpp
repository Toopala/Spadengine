#include "Resources/ModelResource.h"

namespace sge
{
	// Constructor, expects a filepath to a 3D model.
	ModelResource::ModelResource(const std::string& resourcePath) : sge::Resource(resourcePath)
	{
		this->loadModel(resourcePath);
	}
	ModelResource::~ModelResource()
	{
		meshes.erase(meshes.begin(), meshes.end());
	}

	void ModelResource::setRenderer(Renderer* renderer)
	{
		this->renderer = renderer;
	}

	std::vector<Vertex>* ModelResource::getVerticeArray()
	{ 
		return &meshes[0].vertices;
	}
	std::vector<unsigned int>* ModelResource::getIndexArray()
	{ 
		return &meshes[0].indices;
	}
	sge::Texture* ModelResource::getDiffuseTexture()
	{
		for (size_t i = 0; i < meshes[0].textures.size(); i++)
		{
			if (meshes[0].textures[i].getTypeName() == "texture_diffuse")
			{
				return sge::Spade::getInstance().getRenderer()->getDevice()->createTexture(meshes[0].textures[i].getSize().x, meshes[0].textures[i].getSize().y, meshes[0].textures[i].getData());
			}
		}

		return nullptr;
	}
	sge::Texture* ModelResource::getNormalTexture()
	{
		for (size_t i = 0; i < meshes[0].textures.size(); i++)
		{
			if (meshes[0].textures[i].getTypeName() == "texture_normal")
			{
				return sge::Spade::getInstance().getRenderer()->getDevice()->createTexture(meshes[0].textures[i].getSize().x, meshes[0].textures[i].getSize().y, meshes[0].textures[i].getData());
			}
		}

		return nullptr;
	}

	void ModelResource::createBuffers(GraphicsDevice* device)
	{
		for (auto &mesh : meshes)
		{
			mesh.createBuffers(device);
		}
	}

	sge::Buffer* ModelResource::getVertexBuffer()
	{
		return meshes[0].getVertexBuffer();
	}

	sge::Buffer* ModelResource::getIndexBuffer()
	{
		return meshes[0].getIndexBuffer();
	}

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void ModelResource::loadModel(std::string path)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals || aiProcess_CalcTangentSpace);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void ModelResource::processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}

	Mesh ModelResource::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<sge::TextureResource> textures;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			sge::math::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder sge::math::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Tangents
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// Bitangets
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			// UVS
			vector.x = mesh->mTextureCoords[0][i].x;
			vector.y = mesh->mTextureCoords[0][i].y;
			vertex.UV = sge::math::vec2(vector.x, vector.y);

			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<sge::TextureResource> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<sge::TextureResource> specularMaps = this->loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	std::vector<sge::TextureResource> ModelResource::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<sge::TextureResource> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				aiString tempStr;
				tempStr.Set(&textures_loaded[j].getResourcePath()[i]);
				if (tempStr == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				// If texture hasn't been loaded already, load it
				std::string temp(str.C_Str());
				temp = "../Assets/" + temp;
				sge::TextureResource* texture = new sge::TextureResource(temp);
				texture->setTypename(typeName);
				textures.push_back(*texture);
				this->textures_loaded.push_back(*texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};