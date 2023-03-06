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
	}
}

Shader::Shader( ShaderType type, const char* source ) {
	// Compile step
	const auto shader = glCreateShader( ShaderTypeToGLType( type ) );
	glShaderSource( shader, 1, &source, nullptr );
	glCompileShader( shader );

	GLint success = GL_FALSE;

	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetShaderInfoLog( shader, 512, nullptr, msg );
		throw Exception( msg );
	}

	// Link step
	id_ = glCreateProgram();
	glProgramParameteri( id_, GL_PROGRAM_SEPARABLE, GL_TRUE );

	glAttachShader( id_, shader );
	glLinkProgram( id_ );
	glDetachShader( id_, shader );

	success = GL_FALSE;

	glGetProgramiv( id_, GL_LINK_STATUS, &success );
	if ( success == GL_FALSE ) {
		glDeleteShader( shader );

		char msg[512];
		glGetProgramInfoLog( id_, 512, nullptr, msg );
		throw Exception( msg );
	}

	// Don't need the shader object anymore
	glDeleteShader( shader );
}

Shader::~Shader() noexcept {
	glDeleteProgram( id_ );
}

}  // namespace Octave
