#version 410

layout( location=0 ) in vec3 aPos;
layout( location=1 ) in vec3 aColor;
layout( location=3 ) in vec3 aNormal;
layout( location=2 ) in vec2 aTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
};

out FragmentData {
	vec3 vertex_color;
	vec3 vertex_normal;
	vec2 tex_coord;
	vec3 fragment_position;
} vs_out;

layout(std140) uniform Matrices {
	mat4 uMatProjection;
	mat4 uMatView;
	mat4 uMatModel;
	mat3 uMatNormal;
};

void main() {
	gl_Position = uMatProjection * uMatView * uMatModel * vec4( aPos, 1.0 );

	vs_out.vertex_color = aColor;

	vs_out.vertex_normal = uMatNormal * aNormal;

	vs_out.tex_coord = aTexCoord;

	vs_out.fragment_position = vec3( uMatModel * vec4( aPos, 1.0) );
}
