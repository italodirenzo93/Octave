#include "pch/pch.hpp"
#include "IndexBufferGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

IndexBufferGL::IndexBufferGL() {
	glGenBuffers( 1, &id_ );
}

IndexBufferGL::~IndexBufferGL() noexcept {
	glDeleteBuffers( 1, &id_ );
}

uint32_t IndexBufferGL::GetElementCount() const noexcept {
	return element_count_;
}

std::vector<uint32_t> IndexBufferGL::GetData() const noexcept {
	std::vector<uint32_t> data( element_count_ );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glGetBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0,
						static_cast<int>( element_count_ * sizeof( uint32_t ) ),
						data.data() );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	return data;
}

void IndexBufferGL::SetData( const std::vector<uint32_t>& indices ) noexcept {
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				  static_cast<int>( indices.size() * sizeof( uint32_t ) ),
				  reinterpret_cast<const void*>( indices.data() ),
				  GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	element_count_ = static_cast<uint32_t>( indices.size() );
}

}  // namespace Octave::Impl
