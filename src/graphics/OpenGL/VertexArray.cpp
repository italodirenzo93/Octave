#include "VertexArray.hpp"

#include <glad/glad.h>

namespace Octave {

static int AttrNameToIndex( VertexAttributeName name ) noexcept {
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
			assert( false );
			return -1;
	}
}

static GLenum AttrTypeToGLType( VertexAttributeType type ) noexcept {
	switch ( type ) {
		case VertexAttributeType::kFloat:
			return GL_FLOAT;
		case VertexAttributeType::kUbyte:
			return GL_UNSIGNED_BYTE;
		case VertexAttributeType::kUint:
			return GL_UNSIGNED_INT;
		default:
			assert( false );
			return 0;
	}
}

static GLuint AttrTypeSize( VertexAttributeType type ) noexcept {
	switch ( type ) {
		case VertexAttributeType::kFloat:
			return sizeof( GLfloat );
		case VertexAttributeType::kUbyte:
			return sizeof( GLubyte );
		case VertexAttributeType::kUint:
			return sizeof( GLuint );
		default:
			assert( false );
			return 0;
	}
}

VertexArray::VertexArray( const VertexLayout& layout ) : attrs_( layout ) {
	glCreateVertexArrays( 1, &id_ );

	// Define vertex attribute layout
	uint32_t offset = 0;
	for ( const auto& attr : layout ) {
		glVertexArrayAttribFormat(
			id_, AttrNameToIndex( attr.name ), static_cast<GLint>( attr.size ),
			AttrTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ), offset );

		glVertexArrayAttribBinding( id_, AttrNameToIndex( attr.name ), 0 );
		glEnableVertexArrayAttrib( id_, AttrNameToIndex( attr.name ) );

		offset += attr.size * AttrTypeSize( attr.type );
	}
}

VertexArray::~VertexArray() noexcept {
	glDeleteVertexArrays( 1, &id_ );
}

void VertexArray::SetVertexBuffer( uint32_t index, SharedRef<Buffer> vbo ) {
	if ( vbo == nullptr ) {
		glVertexArrayVertexBuffer( id_, index, 0, 0, 0 );
	} else {
		glVertexArrayVertexBuffer( id_, index, vbo->GetApiResource(), 0,
								vbo->GetStride() );
	}
}

void VertexArray::SetIndexBuffer( SharedRef<Buffer> ibo ) {
	if ( ibo == nullptr ) {
		glVertexArrayElementBuffer( id_, 0 );
	} else {
		glVertexArrayElementBuffer( id_, ibo->GetApiResource() );
	}
}

}  // namespace Octave
