#include "model.h"
#include <assimp/postprocess.h>
#include "stb_image.h"
#include <iostream>

Model::Model(const std::string& path)
{
	loadSceneFromFile(path);
}

// Draw the model by sequentially drawing all its constituent meshes
void Model::draw(const Shader& shader, const glm::mat4& trans) const
{
	for (const auto& mesh : Meshes)
		mesh.draw(shader, trans);
}

// Load the scene (collection of meshes) from the given file
void Model::loadSceneFromFile(const std::string& path)
{
	Assimp::Importer importer{};
	
	const auto scene{importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace)};
	
	// Check for errors in the loaded scene
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
		
		return;
	}

	// Cache the directory of the loaded file
	Directory = path.substr(0, path.find_last_of('/'));
	
	getMeshesInNode(scene->mRootNode, scene);
}

// Get and store all meshes from each scene node
void Model::getMeshesInNode(const aiNode* node, const aiScene* scene)
{
	// Process each mesh in the current node
	for (auto i{0}; i < node->mNumMeshes; ++i)
	{
		// Node contains only indices of objects in the scene, so use them retrieve the actual meshes
		const auto mesh{scene->mMeshes[node->mMeshes[i]]};
		Meshes.push_back(storeMeshData(mesh, scene));
	}

	// Repeat above for all sub-nodes (if any)
	for (auto i{0}; i < node->mNumChildren; ++i)
		getMeshesInNode(node->mChildren[i], scene);
}

// Convert retrieved mesh into Mesh object
Mesh Model::storeMeshData(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};
	std::vector<Texture> textures{};

	// Store vertex data
	for (auto i{0}; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex{};

		// Assimps vector class can't be directly convert to a GLM equivalent, so store components individually in a temporary vec3
		glm::vec3 vector{};

		// Vertex positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Vertex normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// Texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texCoords{};

			// Assume model uses only one sets of texture coordinates (out of a possible maximum of eight), so take first set only
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texCoords;

			// Tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// Bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2{0.0f, 0.0f};

		vertices.push_back(vertex);
	}
	
	// Get the indices needed for indexed drawing of each face in the mesh
	for (auto i{0}; i < mesh->mNumFaces; ++i)
	{
		// Indices are stored per face
		const auto face{mesh->mFaces[i]};
		
		// Retrieve all indices of the current face
		for (auto j{0}; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	
	// Get mesh's material data
	const auto material{scene->mMaterials[mesh->mMaterialIndex]};

	// A sampler naming convention in shaders is assumed
	// Diffuse textures should be named "texture_<type>N" where N is between 1 and MAX_SAMPLER_NUMBER and type is one of "diffuse", "specular", and "normal".

	// Diffuse maps
	const auto diffuseMaps{storeMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse")};
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// Specular maps
	const auto specularMaps{storeMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular")};
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// Normal maps
	const auto normalMaps{storeMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal")};
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// Height maps
	const auto heightMaps{storeMaterialTextures(material, aiTextureType_AMBIENT, "texture_height")};
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// Return the completed mesh object containing the retrieved vertices, indices, and textures
	return Mesh{vertices, indices, textures};
}

// Get and store the textures contained within the given material
std::vector<Texture> Model::storeMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures{};

	for (auto i{0}; i < mat->GetTextureCount(type); ++i)
	{
		aiString str{};
		mat->GetTexture(type, i, &str);
		
		// Skip creating objects for textures that have already been loaded and cached
		bool skip{false};
		for (auto j{0}; j < TexturesLoaded.size(); ++j)
		{
			// Textures with identical file paths are assumed to be the same
			if (std::strcmp(TexturesLoaded[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(TexturesLoaded[j]);
				
				skip = true;
				
				break;
			}
		}

		if (!skip)
		{
			// Create objects for new textures
			Texture texture{};
			texture.Id = loadTextureFromFile(str.C_Str(), Directory);
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			
			// Store it as texture loaded for entire model, to prevent unnecessarily loading duplicate textures.
			TexturesLoaded.push_back(texture);  
		}
	}

	// Return the completed texture object(s)
	return textures;
}

// Load a texture from a file and configure it for use in OpenGL
unsigned int Model::loadTextureFromFile(const std::string& path, const std::string& directory, bool gamma)
{
	const auto filename{directory + "/" + path};
	
	unsigned int textureId{};
	
	int width{};
	int height{};
	int numComponents{};

	const auto texData{stbi_load(filename.c_str(), &width, &height, &numComponents, 0)};
	if (texData)
	{
		GLenum format{};

		switch (numComponents)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			throw std::exception{"Invalid number of components.\n"};
		}

		// Generate texture and configure texture
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		std::cout << "Texture failed to load at path: " << path << "\n";

	stbi_image_free(texData);

	// Return the id of the generated OpenGL texture
	return textureId;
}
