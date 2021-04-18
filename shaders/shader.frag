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
	vec3 color;
} light;

uniform vec3 viewPos;

void main()
{
	// Calculate ambient component
	const float ambientStrength = 0.1;
	const vec3 ambient = ambientStrength * light.color;

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
	const vec3 diffuse = difference * light.color;

	// Calculate specular componet
	const float shiniess = 4;
	const float specularStrength = 0.5;
	const vec3 viewDir = normalize(viewPos - vec3(FragPos));
	const vec3 reflectDir = reflect(-lightDir, norm);
	const float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiniess);
	const vec3 specular = specularStrength * spec * light.color;

	// Combine Phong components
	const vec3 result = (ambient + diffuse + specular) * vec3(texture(texture_diffuse1, TexCoords));
	FragColor = vec4(result, 1.0);
}
