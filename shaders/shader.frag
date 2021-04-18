#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPos;

out vec4 FragColor;

// Only diffuse textures are supported at present
uniform sampler2D texture_diffuse1;
//uniform vec3 lightPos;

// Direction light
uniform struct Light
{
	vec4 position; // Only direction is needed for directional lights
	vec3 diffuse;
	vec3 specular;
} light;

void main()
{
	// Hardcoding light pos for now until better solution
	const vec3 lightPos = vec3(10, 10, 0);
	const vec3 lightColor = vec3(1.0); // Light is fully white

	// Calculate ambient component
	const float ambientStrength = 0.1;
	const vec3 ambient = ambientStrength * lightColor;

	// Calculate diffuse component
	const vec3 norm = normalize(Normal);

	// If light has non-zero w component, it's a position, so get the direction between it and the fragment; otherwise just take direction from the light
	vec3 lightDir = vec3(0.0);
	if (light.position.w == 1.0)
	{
		lightDir = vec3(normalize(light.position - FragPos));
	}
	else if (light.position.w == 0.0)
	{
		lightDir = vec3(normalize(-light.position));
	}

	// Direction is defined as from light to frag, but this algo expects the opposite, so swap it
	const float difference = max(dot(norm, lightDir), 0.0);
	const vec3 diffuse = difference * lightColor;

	// Combine Phong components
	const vec3 result = (ambient + diffuse) * vec3(texture(texture_diffuse1, TexCoords));
	FragColor = vec4(result, 1.0);
}
