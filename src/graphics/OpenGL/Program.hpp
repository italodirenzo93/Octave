#ifndef OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP
#define OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP

#include <glm/glm.hpp>

#include "Buffer.hpp"
#include "Shader.hpp"

namespace Octave {

class Program {
public:
	Program() noexcept : handle_( 0 ) {}

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }

	Program& SetBool( const char* name, bool value );
	Program& SetInt( const char* name, int value );
	Program& SetFloat( const char* name, float value );
	Program& SetVec3( const char* name, const glm::vec3& value );
	Program& SetMat4( const char* name, const glm::mat4& value );

	Program& SetUniformBuffer( uint32_t binding, const Buffer& ubo );

private:
	GLuint handle_;
	std::unordered_map<std::string, GLint> uniform_locations_;

	[[nodiscard]] GLint LookupUniform( const char* name ) noexcept;
};

}  // namespace Octave

#endif
