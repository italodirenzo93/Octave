#include "pch/pch.hpp"
#include "BufferGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

static GLenum BindingToGLType( BufferBinding binding ) noexcept {
	switch ( binding ) {
		case BufferBinding::VertexBuffer:
			return GL_ARRAY_BUFFER;
		case BufferBinding::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferBinding::ConstantBuffer:
			return GL_UNIFORM_BUFFER;
		default:
			assert( false );
	}
}

BufferGL::BufferGL( BufferBinding binding, size_t byteWidth )
	: id_( 0 ), binding_( binding ), width_( byteWidth ) {
	assert( byteWidth > 0 );

	glGenBuffers( 1, &id_ );

	Bind();
	glBufferData( BindingToGLType( binding_ ), static_cast<GLsizeiptr>( width_ ),
				  nullptr, GL_STATIC_DRAW );
	Unbind();
}

BufferGL::~BufferGL() noexcept {
	glDeleteBuffers( 1, &id_ );
}

void BufferGL::SetData( const void* data, size_t size ) {
	assert( size <= width_ );

	Bind();
	glBufferSubData( BindingToGLType( binding_ ), 0,
					 static_cast<GLsizeiptr>( size ), data );
	Unbind();
}

void BufferGL::Bind() const noexcept {
	glBindBuffer( BindingToGLType( binding_ ), id_ );
}

void BufferGL::Unbind() const noexcept {
	glBindBuffer( BindingToGLType( binding_ ), 0 );
}

}  // namespace Octave::Impl
