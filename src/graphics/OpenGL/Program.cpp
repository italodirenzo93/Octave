#include "pch/pch.hpp"
#include "Program.hpp"

#include "core/Log.hpp"

namespace Octave {

Program::Program( const Shader& vs, const Shader& fs ) {
	id_ = glCreateProgram();

	glAttachShader( id_, vs.GetApiResource() );
	glAttachShader( id_, fs.GetApiResource() );

	glLinkProgram( id_ );

	glDetachShader( id_, vs.GetApiResource() );
	glDetachShader( id_, fs.GetApiResource() );

	GLint success = GL_FALSE;

	glGetProgramiv( id_, GL_LINK_STATUS, &success );
	if ( success == GL_FALSE ) {
		char msg[512];
		glGetProgramInfoLog( id_, 512, nullptr, msg );
		throw Exception( msg );
	}
}

Program::~Program() noexcept {
	glDeleteProgram( id_ );
}

Program& Program::SetInt( int location, int value ) {
	glProgramUniform1i( id_, location, value );
	return *this;
}

Program& Program::SetFloat( int location, float value ) {
	glProgramUniform1f( id_, location, value );
	return *this;
}

Program& Program::SetVec3( int location, const glm::vec3& value ) {
	glProgramUniform3fv( id_, location, 1, glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetMat4( int location, const glm::mat4& value ) {
	glProgramUniformMatrix4fv( id_, location, 1, GL_FALSE,
							   glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetUniformBuffer( uint32_t binding, SharedRef<Buffer> ubo ) {
	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo->GetApiResource() );

	ubos_[binding] = std::move( ubo );

	return *this;
}

}  // namespace Octave
