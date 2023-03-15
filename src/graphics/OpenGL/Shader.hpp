#ifndef OCTAVE_GRAPHICS_OPENGL_SHADER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADER_HPP

#include <array>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

class Shader {
public:
	using UniformBufferArray =
		std::array<SharedRef<Buffer>, GL_MAX_UNIFORM_BUFFER_BINDINGS>;

	Shader( ShaderType type, const std::string& source );
	~Shader() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	[[nodiscard]] int GetUniformLocation(
		const std::string& name ) const noexcept;
	[[nodiscard]] int GetUniformBlockIndex(
		const std::string& block_name ) const noexcept;

	Shader& SetInt( int location, int value );
	Shader& SetFloat( int location, float value );
	Shader& SetVec3( int location, const glm::vec3& value );
	Shader& SetMat4( int location, const glm::mat4& value );

	Shader& SetUniformBuffer( uint32_t binding, SharedRef<Buffer> ubo );

private:
	GLuint id_ = 0;
	UniformBufferArray ubos_;
};

}

#endif
