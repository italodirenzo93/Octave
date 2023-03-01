#include "pch/pch.hpp"
#include "VertexBufferGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

static GLuint VertexTypeToGLType( VertexAttributeType type ) noexcept {
	switch ( type ) {
		case VertexAttributeType::kFloat:
			return GL_FLOAT;
		case VertexAttributeType::kUint:
			return GL_UNSIGNED_INT;
		case VertexAttributeType::kUbyte:
			return GL_UNSIGNED_BYTE;
		default:
			assert( false );
			return 0;
	}
}

static GLuint GetSizeOfType( uint32_t type ) {
	switch ( type ) {
		case GL_FLOAT:
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		default:
			assert( false );
			return 0;
	}
}

// NOTE: This must be kept in sync with the input layout of the vertex shader!
static GLuint AttributeNameToGLSLBinding( VertexAttributeName name ) noexcept {
	switch ( name ) {
		case VertexAttributeName::kPosition:
			return 0;
		case VertexAttributeName::kColor:
			return 1;
		case VertexAttributeName::kTexCoord:
			return 2;
		case VertexAttributeName::kNormal:
			return 3;
		default:
			return -1;
	}
}

VertexBufferGL::VertexBufferGL() noexcept {
	glGenBuffers( 1, &vbo_ );
	glGenVertexArrays( 1, &vao_ );
}

VertexBufferGL::~VertexBufferGL() noexcept {
	glDeleteVertexArrays( 1, &vao_ );
	glDeleteBuffers( 1, &vbo_ );
}

uint32_t VertexBufferGL::GetVertexCount() const noexcept {
	return vertex_count_;
}

void VertexBufferGL::ReadBufferData( uint32_t vertex_count, size_t vertex_size,
									 void* data ) {
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

	glGetBufferSubData( GL_ARRAY_BUFFER, 0,
						static_cast<int>( vertex_count * vertex_size ), data );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void VertexBufferGL::SetBufferData( const VertexLayout& layout,
									uint32_t vertex_count, size_t vertex_size,
									const void* data ) {
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

	// Set buffer data
	glBufferData( GL_ARRAY_BUFFER,
				  static_cast<int>( vertex_count * vertex_size ), data,
				  GL_STATIC_DRAW );

	// Set up layout
	BindVertexArray( layout, vertex_size );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	vertex_count_ = vertex_count;
}

void VertexBufferGL::BindVertexArray( const VertexLayout& layout,
									  size_t stride ) {
	glBindVertexArray( vao_ );

	size_t offset_in_bytes = 0;

	for ( auto& binding : layout ) {
		const auto binding_point =
			AttributeNameToGLSLBinding( binding.name );
		if ( binding_point == -1 ) continue;

		const auto gl_type = VertexTypeToGLType( binding.type );

		glVertexAttribPointer(
			binding_point, static_cast<int>( binding.size ), gl_type,
			binding.normalized, static_cast<int>( stride ),
			reinterpret_cast<const void*>( offset_in_bytes ) );
		glEnableVertexAttribArray( binding_point );

		offset_in_bytes += binding.size * GetSizeOfType( gl_type );
	}

	layout_ = layout;

	glBindVertexArray( 0 );
}

}  // namespace Octave::Impl
