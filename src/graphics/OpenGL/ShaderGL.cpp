#include "pch/pch.hpp"
#include "ShaderGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

ShaderGL::ShaderGL() {
	id_ = glCreateProgram();
	glProgramParameteri( id_, GL_PROGRAM_SEPARABLE, GL_TRUE );
}

ShaderGL::~ShaderGL() noexcept {
	glDeleteProgram( id_ );
}

void ShaderGL::CompileFromString( const std::string& shader_source ) {
	const auto shader = glCreateShader( GL_VERTEX_SHADER );
	const char* p_shader_source = shader_source.c_str();

	glShaderSource( shader, 1, &p_shader_source, nullptr );
	glCompileShader( shader );

	GLint success = GL_FALSE;

	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( success != GL_TRUE ) {
		glDeleteShader( shader );
		// throw
	}

	// Link step
	glAttachShader( id_, shader );
	glLinkProgram( id_ );
	glDetachShader( id_, shader );

	success = GL_FALSE;

	glGetProgramiv( id_, GL_LINK_STATUS, &success );
	if ( success != GL_TRUE ) {
		glDeleteShader( shader );
		// throw
	}

	glDeleteShader( shader );
}

}  // namespace Octave::Impl
