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

	[[nodiscard]] int GetUniformBlockIndex( const std::string& block_name ) const noexcept;

private:
	GLuint id_ = 0;
};

}

#endif
