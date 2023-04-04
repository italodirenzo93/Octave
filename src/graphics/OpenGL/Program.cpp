#include "pch/pch.hpp"
#include "Program.hpp"

namespace Octave {

Program& Program::SetInt( const char* name, int value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform1i( handle_, LookupUniform( name ), value );
	return *this;
}

Program& Program::SetFloat( const char* name, float value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform1f( handle_, LookupUniform( name ), value );
	return *this;
}

Program& Program::SetVec3( const char* name, const glm::vec3& value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform3fv( handle_, LookupUniform( name ), 1,
						 glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetMat4( const char* name, const glm::mat4& value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniformMatrix4fv( handle_, LookupUniform( name ), 1, GL_FALSE,
							   glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetUniformBuffer( uint32_t binding, const Buffer& ubo ) {
	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo.GetApiResource() );
	return *this;
}

GLint Program::LookupUniform( const char* name ) noexcept {
	const auto it = uniform_locations_.find( name );
	if ( it != uniform_locations_.end() ) {
		return it->second;
	}

	const auto location = glGetUniformLocation( handle_, name );
	if ( location >= 0 ) {
		uniform_locations_.insert_or_assign( name, location );
	}

	return location;
}

}  // namespace Octave
