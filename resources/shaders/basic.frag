#version 410

in FragmentData {
	vec3 vertex_color;
	vec3 vertex_normal;
	vec2 tex_coord;
	vec3 fragment_position;
	vec3 view_position;
} fs_in;

out vec4 FragColor;

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	bool enabled;

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform DirectionalLight uDirectionalLight;

#define MAX_POINT_LIGHTS 4
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

uniform Material uMaterial;

vec3 CalculateDirectionalLighting() {
	vec3 normal = normalize(fs_in.vertex_normal);
	vec3 view_direction = normalize(fs_in.view_position - fs_in.fragment_position);

	vec3 light_direction = normalize(-uDirectionalLight.direction);

	// diffuse shading
	float diffuse_value = max(dot(normal, light_direction), 0.0);

	// specular shading
	vec3 reflection_dir = reflect(-light_direction, normal);
	float specular_value = pow(max(dot(view_direction, reflection_dir), 0.0), uMaterial.shininess);

	// combine results
	vec3 ambient = uDirectionalLight.ambient * vec3(texture(uMaterial.diffuse, fs_in.tex_coord));
	vec3 diffuse = uDirectionalLight.diffuse * diffuse_value * vec3(texture(uMaterial.diffuse, fs_in.tex_coord));
	vec3 specular = uDirectionalLight.specular * specular_value * vec3(texture(uMaterial.specular, fs_in.tex_coord));

	return ambient + diffuse + specular;
}

vec3 CalculatePointLighting(PointLight light) {
	if (!light.enabled) {
		return vec3(0.0, 0.0, 0.0);
	}

	vec3 normal = normalize(fs_in.vertex_normal);
	vec3 light_direction = normalize(light.position - fs_in.fragment_position);
	vec3 viewing_direction = normalize(fs_in.view_position - fs_in.fragment_position);

	// diffuse shading
	float diffuse_value = max(dot(normal, light_direction), 0.0);

	// specular shading
	vec3 reflect_direction = reflect(-light_direction, normal);
	float specular_value = pow(max(dot(viewing_direction, reflect_direction), 0.0), uMaterial.shininess);

	// attenuation
	float distance = length(light.position - fs_in.fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, fs_in.tex_coord));
	vec3 diffuse = light.diffuse * diffuse_value * vec3(texture(uMaterial.diffuse, fs_in.tex_coord));
	vec3 specular = light.specular * specular_value * vec3(texture(uMaterial.specular, fs_in.tex_coord));

	// adjust by attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

void main() {
	vec3 result = CalculateDirectionalLighting();

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		result += CalculatePointLighting(uPointLights[i]);
	}

	FragColor = vec4(result, 1.0);
}
