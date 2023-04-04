#ifndef OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP
#define OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP

#include <vector>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

class VertexArray {
public:
	explicit VertexArray( const VertexLayout& layout ) noexcept
		: handle_( 0 ), attrs_( layout ) {}

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }

	[[nodiscard]] const VertexLayout& GetVertexLayout() const noexcept {
		return attrs_;
	}

private:
	GLuint handle_;
	VertexLayout attrs_;
};

}  // namespace Octave

#endif
