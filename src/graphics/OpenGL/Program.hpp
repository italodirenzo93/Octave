#ifndef OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP
#define OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP

#include <glm/glm.hpp>

#include "core/Types.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace Octave {

struct Program {
	GLuint handle_;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }

	Program& SetInt( int location, int value );
	Program& SetFloat( int location, float value );
	Program& SetVec3( int location, const glm::vec3& value );
	Program& SetMat4( int location, const glm::mat4& value );

	Program& SetUniformBuffer( uint32_t binding, const Buffer& ubo );
};

}  // namespace Octave

#endif
