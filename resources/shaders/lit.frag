#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 uViewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material uMaterial;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight uDirLight;

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight uPointLights[NR_POINT_LIGHTS];

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight uSpotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

	// combine results
	vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(uViewPos - FragPos);

	// phase 1: directional lighting
	vec3 result = CalcDirLight(uDirLight, norm, viewDir);
	// phase 2: point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(uPointLights[i], norm, FragPos, viewDir);
	}
	// phase 3: spot light
	// result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}
