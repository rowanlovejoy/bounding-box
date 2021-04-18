#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec4 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main()
{
	TexCoords = aTexCoords;

	// Convert normal to world space using normal matrix
	Normal = normalMat * aNormal;

	// Convert fragment position to world space before passing it through
	FragPos = model * vec4(aPos, 1.0);

	// Convert vertex position to uniform device coords
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
