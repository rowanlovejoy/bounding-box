#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Pre-cache the model view matrix for subsequent operations
	const mat4 modelView = view * model;

	// Convert fragment position to view space before passing it through
	FragPos = vec3(modelView * vec4(aPos, 1.0));

	// Convert normal to view space using normal matrix
	Normal = transpose(inverse(mat3(modelView))) * aNormal; // Slow, needs to be refactored out of the shader

	TexCoords = aTexCoords;

	// Convert vertex position to uniform device coords
	gl_Position = projection * modelView * vec4(aPos, 1.0);
}
