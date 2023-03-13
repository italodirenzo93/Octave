#ifndef OCTAVE_GRAPHICS_OPENGL_SHADER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADER_HPP

#include <glad/glad.h>

#include "graphics/Structs.hpp"

namespace Octave {

class Shader {
public:
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

private:
	GLuint id_ = 0;
};

}

#endif
