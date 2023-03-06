#ifndef OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP
#define OCTAVE_GRAPHICS_OPENGL_VERTEXARRAY_HPP

#include <vector>
#include <glad/glad.h>

#include "graphics/Structs.hpp"

namespace Octave {

class VertexArray {
public:
	explicit VertexArray( const VertexArrayDescription& desc );
	~VertexArray() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	using Iterator = std::vector<VertexAttribute>::iterator;
	Iterator begin() noexcept { return attrs_.begin(); }
	Iterator end() noexcept { return attrs_.end(); }

private:
	GLuint id_ = 0;
	std::vector<VertexAttribute> attrs_;
};

}

#endif
