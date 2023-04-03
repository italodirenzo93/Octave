#include "pch/pch.hpp"
#include "Shader.hpp"

namespace Octave {

static GLenum ShaderTypeToGLType( ShaderType type ) noexcept {
	switch ( type ) {
		case ShaderType::VertexShader:
			return GL_VERTEX_SHADER;
		case ShaderType::FragmentShader:
			return GL_FRAGMENT_SHADER;
		default:
			assert( false );
			return 0;
	}
}

Shader::Shader( ShaderType type, const std::string& source ) {
	// Compile step
	id_ = glCreateShader( ShaderTypeToGLType( type ) );
	const char* shader_source = source.c_str();

	glShaderSource( id_, 1, &shader_source, nullptr );
	glCompileShader( id_ );

	GLint success = GL_FALSE;

	glGetShaderiv( id_, GL_COMPILE_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetShaderInfoLog( id_, 512, nullptr, msg );
		throw Exception( msg );
	}
}

Shader::~Shader() noexcept {
	glDeleteShader( id_ );
}

}  // namespace Octave
