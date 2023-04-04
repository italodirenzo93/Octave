#ifndef OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP
#define OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP

#include <vector>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

struct VertexArray {
	GLuint handle_ = 0;
	VertexLayout attrs_;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }
};

}

#endif
