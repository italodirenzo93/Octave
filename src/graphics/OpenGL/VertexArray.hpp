#ifndef OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP
#define OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP

#include <vector>
#include <glad/glad.h>

#include "graphics/Structs.hpp"
#include "Buffer.hpp"

namespace Octave {

class VertexArray {
public:
	explicit VertexArray( const VertexLayout& layout );
	~VertexArray() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	void SetVertexBuffer( uint32_t index, SharedRef<Buffer> vbo );
	void SetIndexBuffer( SharedRef<Buffer> ibo );

	using Iterator = VertexLayout::iterator;
	Iterator begin() noexcept { return attrs_.begin(); }
	Iterator end() noexcept { return attrs_.end(); }

private:
	GLuint id_ = 0;
	VertexLayout attrs_;
};

}

#endif
