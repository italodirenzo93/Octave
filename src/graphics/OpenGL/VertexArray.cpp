#include "VertexArray.hpp"

#include <glad/glad.h>

namespace Octave {

VertexArray::VertexArray( const VertexArrayDescription& desc )
	: attrs_( desc.attributes, desc.attributes + desc.count ) {
	glGenVertexArrays( 1, &id_ );
}

VertexArray::~VertexArray() noexcept {
	glDeleteVertexArrays( 1, &id_ );
}

}  // namespace Octave
