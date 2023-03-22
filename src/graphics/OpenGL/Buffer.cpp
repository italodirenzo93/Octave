#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

static GLint AccessToGLBufferBit( ResourceAccess access ) noexcept {
	switch ( access ) {
		default:
		case ResourceAccess::Read:
			return 0;
		case ResourceAccess::ReadWrite:
			return GL_DYNAMIC_STORAGE_BIT;
	}
}

Buffer::Buffer( const BufferDescription& desc, const void* initial_data )
	: id_( 0 ), desc_( desc ) {
	glCreateBuffers( 1, &id_ );

	glNamedBufferStorage( id_, static_cast<GLsizeiptr>( desc.size ),
						  initial_data,
						  AccessToGLBufferBit( desc.access_flags ) );
}

Buffer::~Buffer() noexcept {
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

void Buffer::SetData( const void* data, uint32_t size ) {
	assert( data != nullptr && size <= desc_.size );
	glNamedBufferSubData( id_, 0, static_cast<GLsizeiptr>( size ), data );
}

}  // namespace Octave
