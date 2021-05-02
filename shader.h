#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/fwd.hpp>
#include <string>

// Class representing a shader program composed of a vertex and fragment shader, and optionally a geometry shader. Handles loading, compiling, and linking shader code from file, setting uniform values, and activating the shader program for use. Declaration and implementation code is based on example provided by LearnOpenGL.com - source: https://learnopengl.com/Getting-started/Shaders
class Shader
{
public:
	// Compile and link the given shaders to a create a usable shader program
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string{});

	// Active the shader for use -- shader must be active before setting uniform values
	void use() const;

	// Utility functions to set shader uniform values
	void setUniform(const std::string& name, bool value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, const glm::vec2& value) const;
	void setUniform(const std::string& name, float x, float y) const;
	void setUniform(const std::string& name, const glm::vec3& value) const;
	void setUniform(const std::string& name, float x, float y, float z) const;
	void setUniform(const std::string& name, const glm::vec4& value) const;
	void setUniform(const std::string& name, float x, float y, float z, float w) const;
	void setUniform(const std::string& name, const glm::mat2& value) const;
	void setUniform(const std::string& name, const glm::mat3& value) const;
	void setUniform(const std::string& name, const glm::mat4& value) const;

	// Get the id of the shader program
	unsigned int getId() const
	{
		return Id;
	}

private:
	unsigned int Id;

	// Check for shader compilation and linking errors 
	static void checkShaderErrors(const GLuint& shader, const std::string& type);
};
