#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

static GLint AccessToGLBufferBit( ResourceAccess access ) noexcept {
	switch ( access ) {
		case ResourceAccess::Read:
			return GL_MAP_READ_BIT;
		case ResourceAccess::Write:
			return GL_MAP_WRITE_BIT;
		default:
		case ResourceAccess::Copy:
			return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
	}
}

Buffer::Buffer( const BufferDescription& desc, const void* initial_data )
	: id_( 0 ), desc_( desc ), mapped_data_( nullptr ) {
	glCreateBuffers( 1, &id_ );

	const auto access_flags = AccessToGLBufferBit( desc.access_flags ) |
							  GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glNamedBufferStorage( id_, static_cast<GLsizeiptr>( desc.size ),
						  initial_data, access_flags );

	mapped_data_ = glMapNamedBufferRange( id_, 0, desc.size, access_flags );
}

Buffer::~Buffer() noexcept {
	glUnmapNamedBuffer( id_ );
	glDeleteBuffers( 1, &id_ );
}

uint32_t Buffer::GetSize() const noexcept {
	return desc_.size;
}

uint32_t Buffer::GetStride() const noexcept {
	return desc_.stride;
}

uint32_t Buffer::GetNumElements() const noexcept {
	if ( desc_.stride == 0 ) {
		return desc_.size;
	}

	return desc_.size / desc_.stride;
}

void Buffer::SetMappedData( const void* data, uint32_t size ) {
	assert( mapped_data_ != nullptr && data != nullptr && size <= desc_.size );
	memcpy_s( mapped_data_, desc_.size, data, size );
}

}  // namespace Octave
