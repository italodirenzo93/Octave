#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

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

void Buffer::SetData( const void* data, uint32_t offset, uint32_t size ) {
	assert( data != nullptr && size <= desc_.size && offset < size );

	glBindBuffer( target_, handle_ );

	glBufferSubData( target_, offset, static_cast<GLsizeiptr>( size ), data );

	glBindBuffer( target_, 0 );
}

}  // namespace Octave
