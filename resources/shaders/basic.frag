#version 410

in vec3 VertexColor;
in vec3 VertexNormal;
in vec2 TexCoord;
in vec3 FragPosition;

out vec4 FragColor;

uniform vec3 uViewPos;

// texture 0 = diffuse map
// texture 1 = specular map
// texture 2 = normal map
#define MAX_TEXTURE_COUNT 3
uniform sampler2D uTextures[MAX_TEXTURE_COUNT];

uniform float uShininess = 32.0;

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectionalLight uDirectionalLight;

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
#define MAX_POINT_LIGHTS 4
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

vec3 CalculateDirectionalLighting() {
	vec3 normal = normalize(VertexNormal);
	vec3 view_direction = normalize(uViewPos - FragPosition);

	vec3 light_direction = normalize(- uDirectionalLight.direction);

	// diffuse shading
	float diffuse_value = max(dot(normal, light_direction), 0.0);

	// specular shading
	vec3 reflection_dir = reflect(-light_direction, normal);
	float specular_value = pow(max(dot(view_direction, reflection_dir), 0.0), uShininess);

	// combine results
	vec3 ambient = uDirectionalLight.ambient * vec3(texture(uTextures[0], TexCoord));
	vec3 diffuse = uDirectionalLight.diffuse * diffuse_value * vec3(texture(uTextures[0], TexCoord));
	vec3 specular = uDirectionalLight.specular * specular_value * vec3(texture(uTextures[1], TexCoord));

	return ambient + diffuse + specular;
}

vec3 CalculatePointLighting( PointLight light ) {
	if ( !light.enabled ) {
		return vec3( 0.0, 0.0, 0.0 );
	}

	vec3 normal = normalize( VertexNormal );
	vec3 light_direction = normalize( light.position - FragPosition );
	vec3 viewing_direction = normalize( uViewPos - FragPosition );

	// diffuse shading
	float diffuse_value = max( dot( normal, light_direction), 0.0 );

	// specular shading
	vec3 reflect_direction = reflect( -light_direction, normal );
	float specular_value = pow( max( dot( viewing_direction, reflect_direction), 0.0 ), uShininess );

	// attenuation
	float distance = length( light.position - FragPosition );
	float attenuation = 1.0 / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

	// combine results
	vec3 ambient = light.ambient * vec3( texture( uTextures[0], TexCoord ) );
	vec3 diffuse = light.diffuse * diffuse_value * vec3( texture( uTextures[0], TexCoord ) );
	vec3 specular = light.specular * specular_value * vec3( texture( uTextures[1], TexCoord ) );

	// adjust by attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

void main() {
	vec3 result = CalculateDirectionalLighting();

	for ( int i = 0; i < MAX_POINT_LIGHTS; i++ ) {
		result += CalculatePointLighting( uPointLights[i] );
	}

	FragColor = vec4(result, 1.0);
}
