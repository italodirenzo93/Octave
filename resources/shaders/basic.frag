#version 330 core

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

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light uLight;

vec3 CalculateLighting() {
	vec3 normal = normalize( VertexNormal );
	vec3 view_direction = normalize( uViewPos - FragPosition );

	vec3 light_direction = normalize( -uLight.direction );

	// diffuse shading
	float diffuse_value = max( dot( normal, light_direction ), 0.0 );

	// specular shading
	vec3 reflection_dir = reflect( -light_direction, normal );
	float specular_value = pow( max( dot( view_direction, reflection_dir ), 0.0 ), uShininess );

	// combine results
	vec3 ambient = uLight.ambient * vec3( texture( uTextures[0], TexCoord ) );
	vec3 diffuse = uLight.diffuse * diffuse_value * vec3( texture( uTextures[0], TexCoord ) );
	vec3 specular = uLight.specular * specular_value * vec3( texture( uTextures[1], TexCoord ) );

	return ambient + diffuse + specular;
}

void main() {
	vec3 result = CalculateLighting();

	FragColor = vec4( result, 1.0 );
}
