#ifndef OCTAVE_GRAPHICS_OPENGL_SHADER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADER_HPP

#include <array>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

struct Shader {
	GLuint handle_;
	GLenum type_;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }
};

}

#endif
