#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube uCubemap;

void main() {
    FragColor = texture(uCubemap, TexCoords);
}
