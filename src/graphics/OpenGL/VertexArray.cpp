#include "VertexArray.hpp"

#include <glad/glad.h>

namespace Octave {

VertexArray::VertexArray( const VertexLayout& layout )
	: attrs_( layout ) {
	glGenVertexArrays( 1, &id_ );
}

VertexArray::~VertexArray() noexcept {
	glDeleteVertexArrays( 1, &id_ );
}

}  // namespace Octave
