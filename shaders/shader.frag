#version 450 core

in vec2 TexCoords;

out vec4 FragColor;

// Only diffuse textures are supported at present
uniform sampler2D texture_diffuse1;

void main()
{
	// Add code for handling specular , normal, and height maps

	FragColor = texture(texture_diffuse1, TexCoords);
}
