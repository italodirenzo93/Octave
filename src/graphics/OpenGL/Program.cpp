#include "pch/pch.hpp"
#include "Program.hpp"

#include "core/Log.hpp"

namespace Octave {

Program& Program::SetInt( int location, int value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform1i( handle_, location, value );
	return *this;
}

Program& Program::SetFloat( int location, float value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform1f( handle_, location, value );
	return *this;
}

Program& Program::SetVec3( int location, const glm::vec3& value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniform3fv( handle_, location, 1, glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetMat4( int location, const glm::mat4& value ) {
	assert( glIsProgram( handle_ ) );
	glProgramUniformMatrix4fv( handle_, location, 1, GL_FALSE,
							   glm::value_ptr( value ) );
	return *this;
}

Program& Program::SetUniformBuffer( uint32_t binding, const Buffer& ubo ) {
	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo.GetApiResource() );
	return *this;
}

}  // namespace Octave
