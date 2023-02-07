#version 410

layout( location=0 ) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uMatProjection;
uniform mat4 uMatView;

void main() {
	vec4 pos = uMatProjection * uMatView * vec4( aPos, 1.0 );
	gl_Position = pos.xyww;

	TexCoords = aPos;
}
