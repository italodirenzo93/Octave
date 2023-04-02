#include "pch/pch.hpp"
#include "Buffer.hpp"

namespace Octave {

static GLint BindingToGLTarget( BufferBinding binding ) noexcept {
	switch ( binding ) {
		default:
		case BufferBinding::VertexBuffer: return GL_ARRAY_BUFFER;
		case BufferBinding::IndexBuffer: return GL_ELEMENT_ARRAY_BUFFER;
		case BufferBinding::UniformBuffer: return GL_UNIFORM_BUFFER;
	}
}

static GLenum AccessToGLBufferUsage( ResourceAccess access ) noexcept {
	switch ( access ) {
		default:
		case ResourceAccess::Read: return GL_STATIC_DRAW;
		case ResourceAccess::ReadWrite: return GL_DYNAMIC_DRAW;
	}
}

static GLint AccessToGLBufferStorageBit( ResourceAccess access ) noexcept {
	switch ( access ) {
		default:
		case ResourceAccess::Read:
			return 0;
		case ResourceAccess::ReadWrite:
			return GL_DYNAMIC_STORAGE_BIT;
	}
}

Buffer::Buffer( const BufferDescription& desc, const void* initial_data )
	: id_( 0 ), target_( BindingToGLTarget( desc.bind_flags ) ), desc_( desc ) {
	glGenBuffers( 1, &id_ );

	glBindBuffer( target_, id_ );

	if ( GLAD_GL_ARB_buffer_storage ) {
		glBufferStorage( target_, static_cast<GLsizeiptr>( desc.size ),
						  initial_data,
						  AccessToGLBufferStorageBit( desc.access_flags ) );
	} else {
		glBufferData( target_, static_cast<GLsizeiptr>( desc.size ), initial_data, AccessToGLBufferUsage( desc.access_flags ) );
	}

	glBindBuffer( target_, 0 );
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

	glBindBuffer( target_, id_ );

	glBufferSubData( target_, 0, static_cast<GLsizeiptr>( size ), data );

	glBindBuffer( target_, 0 );
}

}  // namespace Octave
