#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

static GLenum BindingToGLType( BufferBinding binding ) noexcept {
	switch ( binding ) {
		case BufferBinding::VertexBuffer:
			return GL_ARRAY_BUFFER;
		case BufferBinding::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferBinding::UniformBuffer:
			return GL_UNIFORM_BUFFER;
		default:
			assert( false );
	}
}

static GLenum AccessToGLType( ResourceAccess access ) noexcept {
	switch ( access ) {
		case ResourceAccess::Copy:
			return GL_STATIC_COPY;
		case ResourceAccess::Read:
			return GL_STATIC_READ;
		case ResourceAccess::Write:
			return GL_STATIC_DRAW;
		default:
			assert( false );
	}
}

Buffer::Buffer( const BufferDescription& desc, const void* initial_data )
	: desc_( desc ) {
	glCreateBuffers( 1, &id_ );

	glNamedBufferData( id_, static_cast<GLsizeiptr>( desc.size ), initial_data,
				  AccessToGLType( desc.access_flags ) );
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

void Buffer::SetData( uint32_t offset, uint32_t size, const void* data ) {
	glNamedBufferSubData( id_, static_cast<GLintptr>( offset ),
						  static_cast<GLsizeiptr>( size ), data );
}

}  // namespace Octave
