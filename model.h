#pragma once

#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>
#include <vector>

// Class representing a 3D model composed of one or meshes. Handles loading model data from file using Assimp and rendering constituent meshes. Declaration and implementation code is based on example provided by LearnOpenGL.com - source: https://learnopengl.com/Model-Loading/Model
class Model
{
public:
	Model(const std::string& path);

	void draw(const Shader& shader, const glm::mat4& trans) const;

private:
	std::vector<Texture> TexturesLoaded{};
	std::vector<Mesh> Meshes{};
	std::string Directory{};

	void loadSceneFromFile(const std::string& path);

	void getMeshesInNode(const aiNode* node, const aiScene* scene);

	Mesh storeMeshData(const aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> storeMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

	static unsigned int loadTextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
};
