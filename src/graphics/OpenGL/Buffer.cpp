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

static GLenum AccessToGLType( BufferAccess access ) noexcept {
	switch ( access ) {
		case BufferAccess::Copy:
			return GL_STATIC_COPY;
		case BufferAccess::Read:
			return GL_STATIC_READ;
		case BufferAccess::Write:
			return GL_STATIC_DRAW;
		default:
			assert( false );
	}
}

Buffer::Buffer( const BufferDescription& desc, const void* initial_data )
	: desc_( desc ) {
	glGenBuffers( 1, &id_ );

	const auto target = BindingToGLType( desc.bind_flags );

	glBindBuffer( target, id_ );
	glBufferData( target, static_cast<GLsizeiptr>( desc.size ), initial_data,
				  AccessToGLType( desc.access_flags ) );
	glBindBuffer( target, 0 );
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

}  // namespace Octave
