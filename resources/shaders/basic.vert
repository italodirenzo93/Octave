#version 330 core

layout( location=0 ) in vec3 aPos;
layout( location=1 ) in vec3 aColor;
layout( location=3 ) in vec3 aNormal;
layout( location=2 ) in vec2 aTexCoord;

out vec3 VertexColor;
out vec3 VertexNormal;
out vec2 TexCoord;
out vec3 FragPosition;

uniform mat4 uMatProjection;
uniform mat4 uMatView;
uniform mat4 uMatModel;

void main() {
	gl_Position = uMatProjection * uMatView * uMatModel * vec4( aPos, 1.0 );

	VertexColor = aColor;

	VertexNormal = mat3( transpose( inverse( uMatModel) ) ) * aNormal;

	TexCoord = aTexCoord;

	FragPosition = vec3( uMatModel * vec4( aPos, 1.0) );
}
