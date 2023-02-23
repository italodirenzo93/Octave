#version 330 core

in vec3 VertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
	vec3 result = vec3(texture(uTexture, TexCoord));

	FragColor = vec4(result, 1.0);
}
