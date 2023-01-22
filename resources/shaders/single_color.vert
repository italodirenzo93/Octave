#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor; // currently unused
layout(location=2) in vec2 aTexCoords;
layout(location=3) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uMatProjection;
uniform mat4 uMatView;
uniform mat4 uMatModel;
uniform mat4 uMatNormal;

void main() {
    gl_Position = uMatProjection * uMatView * uMatModel * vec4(aPos, 1.0);
    FragPos = vec3(uMatModel * vec4(aPos, 1.0));
    Normal = mat3(uMatNormal) * aNormal;
    TexCoords = aTexCoords;
}
