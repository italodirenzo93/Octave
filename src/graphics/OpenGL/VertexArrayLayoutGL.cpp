#include "pch/pch.hpp"
#include "VertexArrayLayoutGL.hpp"

namespace Octave::Impl {

static GLuint AttributeNameToLocation( VertexAttributeName name ) noexcept {
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
	}
}

static GLenum AttributeTypeToGLType( VertexAttributeType type ) noexcept {
	switch ( type ) {
		case VertexAttributeType::kFloat:
			return GL_FLOAT;
		case VertexAttributeType::kUbyte:
			return GL_UNSIGNED_BYTE;
		case VertexAttributeType::kUint:
			return GL_UNSIGNED_INT;
		default:
			assert( false );
	}
}

bool VertexAttributeGL::operator==(
	const VertexAttributeGL& other ) const noexcept {
	return index == other.index && size == other.size && type == other.type &&
		   normalized == other.normalized;
}

VertexArrayLayoutGL::VertexArrayLayoutGL() noexcept {
	glGenVertexArrays( 1, &id_ );
}

VertexArrayLayoutGL::~VertexArrayLayoutGL() noexcept {
	glDeleteVertexArrays( 1, &id_ );
}

void VertexArrayLayoutGL::Bind() const noexcept {
	glBindVertexArray( id_ );
}

void VertexArrayLayoutGL::Unbind() const noexcept {
#ifdef OGT_DEBUG
	glBindVertexArray( 0 );
#endif
}

void VertexArrayLayoutGL::SetAttributes(
	const std::vector<VertexAttribute>& attributes ) {
	attributes_.clear();
	for ( const auto& attr : attributes ) {
		VertexAttributeGL gl_attr = {
			AttributeNameToLocation( attr.name ), attr.size,
			AttributeTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ) };
		attributes_.emplace( gl_attr );
	}
}

void VertexArrayLayoutGL::AddAttribute( VertexAttribute attribute ) {
	VertexAttributeGL gl_attr = {
		AttributeNameToLocation( attribute.name ), attribute.size,
		AttributeTypeToGLType( attribute.type ),
		static_cast<GLboolean>( attribute.normalized ) };
	attributes_.emplace( gl_attr );
}

}  // namespace Octave::Impl
