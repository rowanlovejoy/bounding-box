#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures)
{
	this->Vertices = vertices;
	this->Indices = indices;
	this->Textures = textures;

	setUpMesh();
}

// Render the mesh using the provided shader and transform
void Mesh::draw(const Shader& shader, const glm::mat4& transform) const
{
	// First texture of each type will have the index 1
	unsigned int diffuseNr{1};
	unsigned int specularNr{1};
	unsigned int normalNr{1};
	unsigned int heightNr{1};

	shader.use();
	
	// Set sampler in shader and bind texture for rendering
	for (auto i{0}; i < Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		// Get texture index based on number of existing textures of its type
		const auto name{Textures[i].Type};
		std::string number{};
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		shader.setUniform(name + number, i);
		
		glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
	}

	shader.setUniform("model", transform);

	// Render the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

// Configure OpenGL buffers and attribute pointers needed for drawing the mesh
void Mesh::setUpMesh()
{
	// Generate vertex array and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO to store subsequent VBO and attribute pointer configuration
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Since structs are sequential, a pointer to struct functions like a pointer to the first element of an array, meaning it can be passed to indicate the start of the data vertex
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// Configure vertex attribute pointers
	
	constexpr auto stride{sizeof(Vertex)};
	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
	
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	
	// Texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
	
	// Vertex tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
	
	// Vertex bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));

	// Unbind VAO once configuration is finished
	glBindVertexArray(0);
}
