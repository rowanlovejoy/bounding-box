#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aTexCoords;

	const mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vPosition, 1.0);
}
