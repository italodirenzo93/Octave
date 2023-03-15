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

Shader::Shader( ShaderType type, const std::string& source ) {
	// Compile step
	const auto shader = glCreateShader( ShaderTypeToGLType( type ) );
	const char* shader_source = source.c_str();

	glShaderSource( shader, 1, &shader_source, nullptr );
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

	success = GL_FALSE;

	glGetProgramiv( id_, GL_LINK_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetProgramInfoLog( id_, 512, nullptr, msg );

		glDetachShader( id_, shader );
		glDeleteShader( shader );

		throw Exception( msg );
	}

	// Don't need the shader object anymore
	glDetachShader( id_, shader );
	glDeleteShader( shader );
}

Shader::~Shader() noexcept {
	glDeleteProgram( id_ );
}

int Shader::GetUniformLocation( const std::string& name ) const noexcept {
	return glGetUniformLocation( id_, name.c_str() );
}


int Shader::GetUniformBlockIndex( const std::string& block_name ) const noexcept {
	return glGetUniformBlockIndex( id_, block_name.c_str() );
}

Shader& Shader::SetInt( int location, int value ) {
	glProgramUniform1i( id_, location, value );
	return *this;
}

Shader& Shader::SetFloat( int location, float value ) {
	glProgramUniform1f( id_, location, value );
	return *this;
}

Shader& Shader::SetVec3( int location, const glm::vec3& value ) {
	glProgramUniform3fv( id_, location, 1, glm::value_ptr( value ) );
	return *this;
}

Shader& Shader::SetMat4( int location, const glm::mat4& value ) {
	glProgramUniformMatrix4fv( id_, location, 1, GL_FALSE,
							   glm::value_ptr( value ) );
	return *this;
}

Shader& Shader::SetUniformBuffer( uint32_t binding, SharedRef<Buffer> ubo ) {
	assert( binding < GL_MAX_UNIFORM_BUFFER_BINDINGS );

	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo->GetApiResource() );

	ubos_[binding] = std::move( ubo );

	return *this;
}

}  // namespace Octave
