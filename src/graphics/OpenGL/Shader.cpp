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

Shader::Shader() {
	id_ = glCreateProgram();
	glProgramParameteri( id_, GL_PROGRAM_SEPARABLE, GL_TRUE );
}

Shader::~Shader() noexcept {
	glDeleteProgram( id_ );
}

}  // namespace Octave
