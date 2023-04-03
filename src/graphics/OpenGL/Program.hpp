#ifndef OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP
#define OCTAVE_GRAPHICS_OPENGL_PROGRAM_HPP

#include <glm/glm.hpp>

#include "core/Types.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace Octave {

class Program {
public:
	using UniformBufferArray =
		std::array<SharedRef<Buffer>, GL_MAX_UNIFORM_BUFFER_BINDINGS>;

	Program( const Shader& vs, const Shader& fs );
	~Program() noexcept;

	Program() = delete;
	Program( const Program& ) = delete;
	Program& operator=( const Program& ) = delete;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	Program& SetInt( int location, int value );
	Program& SetFloat( int location, float value );
	Program& SetVec3( int location, const glm::vec3& value );
	Program& SetMat4( int location, const glm::mat4& value );

	Program& SetUniformBuffer( uint32_t binding, SharedRef<Buffer> ubo );

private:
	GLuint id_ = 0;
	UniformBufferArray ubos_;
};

}  // namespace Octave

#endif
