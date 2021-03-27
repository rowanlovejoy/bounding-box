#include "shader.h"
#include <glm/mat4x4.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	std::string vertexCode{};
	std::string fragmentCode{};
	std::string geometryCode{};

	// Read contents of shader files into memory
	try
	{
		std::ifstream vShaderFile{};
		std::ifstream fShaderFile{};
		std::ifstream gShaderFile{};

		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream{};
		std::stringstream fShaderStream{};

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// Also load a geometry shader if provided
		if (!geometryPath.empty())
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream{};
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << "\n";
	}

	const auto vShaderCode{vertexCode.c_str()};
	const auto fShaderCode{fragmentCode.c_str()};

	// Compile shaders
	const auto vertex{glCreateShader(GL_VERTEX_SHADER)};
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkShaderErrors(vertex, "VERTEX");
	
	const auto fragment{glCreateShader(GL_FRAGMENT_SHADER)};
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkShaderErrors(fragment, "FRAGMENT");

	// Also compile geometry shader if provided
	unsigned int geometry{};
	if (!geometryPath.empty())
	{
		const auto gShaderCode{geometryCode.c_str()};
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);
		checkShaderErrors(geometry, "GEOMETRY");
	}

	// Link compiled shaders into shader program
	Id = glCreateProgram();
	glAttachShader(Id, vertex);
	glAttachShader(Id, fragment);
	if (!geometryPath.empty())
		glAttachShader(Id, geometry);
	glLinkProgram(Id);
	checkShaderErrors(Id, "PROGRAM");

	// Cleanup
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!geometryPath.empty())
		glDeleteShader(geometry);
}

void Shader::use() const
{
	glUseProgram(Id);
}

void Shader::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), static_cast<int>(value));
}

void Shader::setUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
}

void Shader::setUniform(const std::string& name, const glm::mat2& value) const
{
	glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::checkShaderErrors(const GLuint& shader, const std::string& type)
{
	constexpr int logLength{1024};

	int success{};
	char infoLog[logLength]{};

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, logLength, nullptr, infoLog);
			
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n\n";
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, logLength, nullptr, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n\n";
		}
	}
}
