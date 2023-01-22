#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
layout(location=2) in vec2 aTexCoord;

out vec3 VertexColor;
out vec2 TexCoord;

uniform mat4 uMatModel;
uniform mat4 uMatView;
uniform mat4 uMatProjection;

void main() {
	gl_Position = uMatProjection * uMatView * uMatModel * vec4(aPos, 1.0);
	VertexColor = aColor;
	TexCoord = aTexCoord;
}
