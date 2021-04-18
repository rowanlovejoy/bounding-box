#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

// Only diffuse textures are supported at present
uniform sampler2D texture_diffuse1;
//uniform vec3 lightPos;

// Direction light
uniform struct DirLight
{
	vec3 direction; // Only direction is needed for directional lights
	vec3 diffuse;
	vec3 specular;
} dirLight;

void main()
{
	// Hardcoding light pos for now until better solution
	const vec3 lightPos = vec3(10, 10, 0);
	const vec3 lightColor = vec3(1.0); // Light is fully white

	// Add code for handling specular , normal, and height maps

	// Calculate ambient component
	const float ambientStrength = 0.1;
	const vec3 ambient = ambientStrength * lightColor;

	// Calculate diffuse component
	const vec3 norm = normalize(Normal);
	const vec3 lightDir = normalize(lightPos - FragPos);
	//const vec3 lightDir = normalize(-dirLight.direction); // Direction is defined as from light to frag, but this algo expects the opposite, so swap it
	const float difference = max(dot(norm, lightDir), 0.0);
	const vec3 diffuse = difference * lightColor;

	// Combine Phong components
	const vec3 result = (ambient + diffuse) * vec3(texture(texture_diffuse1, TexCoords));
	FragColor = vec4(result, 1.0);
}
