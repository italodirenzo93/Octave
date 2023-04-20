#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

Buffer::Buffer( const BufferDescription& desc ) noexcept
	: handle_( 0 ), target_( GL_ARRAY_BUFFER ), desc_( desc ) {
	switch ( desc.m_Type ) {
		default:
		case BufferType::VertexBuffer:
			target_ = GL_ARRAY_BUFFER;
			break;
		case BufferType::IndexBuffer:
			target_ = GL_ELEMENT_ARRAY_BUFFER;
			break;
		case BufferType::UniformBuffer:
			target_ = GL_UNIFORM_BUFFER;
			break;
	}
}

uint32_t Buffer::GetSize() const noexcept {
	return desc_.m_Size;
}

uint32_t Buffer::GetStride() const noexcept {
	return desc_.m_Stride;
}

uint32_t Buffer::GetNumElements() const noexcept {
	if ( desc_.m_Stride == 0 ) {
		return desc_.m_Size;
	}

	return desc_.m_Size / desc_.m_Stride;
}

void Buffer::SetData( const void* data, uint32_t offset, uint32_t size ) {
	assert( data != nullptr && size <= desc_.m_Size && offset < size );

	glBindBuffer( target_, handle_ );

	glBufferSubData( target_, offset, static_cast<GLsizeiptr>( size ), data );

	glBindBuffer( target_, 0 );
}

}  // namespace Octave
