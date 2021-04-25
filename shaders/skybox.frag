#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPos;

out vec4 FragColor;

// Only diffuse textures are supported at present
uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}
