#include "pch/pch.hpp"
#include "graphics/GraphicsContext.hpp"

#include <glad/glad.h>

#include <array>

#include "Config.hpp"

using namespace std;

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
	}
}

static GLint max_indices = 0;

GraphicsContext::GraphicsContext() {
	if ( max_indices == 0 ) {
		glGetIntegerv( GL_MAX_ELEMENTS_INDICES, &max_indices );
	}

	const Config& config = Config::Instance();

	// Enable depth by default
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0 );

	// Default culling options
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );
	if ( config.IsCullFaceEnabled() ) {
		glEnable( GL_CULL_FACE );
	}
}

GraphicsContext::~GraphicsContext() noexcept {
}

void GraphicsContext::Clear( bool color, bool depth, float r, float g,
							   float b, float a ) const noexcept {
	int clear_flags = 0;

	if ( color ) {
		clear_flags |= GL_COLOR_BUFFER_BIT;
		glClearColor( r, g, b, a );
	}

	if ( depth ) {
		clear_flags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth( 1.0 );
	}

	glClear( clear_flags );
}

void GraphicsContext::Draw( size_t vertex_count,
							  size_t offset ) const noexcept {
	glDrawArrays( GL_TRIANGLES, static_cast<GLint>( offset ),
				  static_cast<GLsizei>( vertex_count ) );
}

void GraphicsContext::DrawIndexed( size_t index_count, size_t offset,
									 size_t base_vertex ) const noexcept {
	glDrawRangeElementsBaseVertex(
		GL_TRIANGLES, static_cast<GLuint>( offset ), max_indices,
		static_cast<GLsizei>( index_count ), GL_UNSIGNED_SHORT, nullptr,
		static_cast<GLint>( base_vertex ) );
}

std::array<int, 4> GraphicsContext::GetViewport() const noexcept {
	std::array<int, 4> vp;
	glGetIntegerv( GL_VIEWPORT, vp.data() );
	return vp;
}

void GraphicsContext::SetDepthTestEnabled( bool enabled ) noexcept {
	if ( enabled ) {
		glEnable( GL_DEPTH_TEST );
	} else {
		glDisable( GL_DEPTH_TEST );
	}
}

void GraphicsContext::SetVertexBuffer( SharedRef<Buffer> vbo,
										 SharedRef<VertexArray> vao ) {
	// Define vertex data layout
	glVertexArrayVertexBuffer( vao->GetApiResource(), 0,
							   vbo->GetApiResource(), 0,
							   vbo->GetStride() );

	uint32_t offset = 0;

	for ( const auto& attr : *vao ) {
		glVertexArrayAttribFormat(
			vao->GetApiResource(), AttrNameToIndex( attr.name ),
			static_cast<GLint>( attr.size ), AttrTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ), offset );

		glVertexArrayAttribBinding( vao->GetApiResource(), AttrNameToIndex( attr.name ), 0 );
		glEnableVertexArrayAttrib( vao->GetApiResource(), AttrNameToIndex( attr.name ) );

		offset += attr.size * AttrTypeSize( attr.type );
	}

	// Bind for draw
	glBindVertexArray( vao->GetApiResource() );
}

void GraphicsContext::SetIndexBuffer( SharedRef<Buffer> ibo ) {
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo->GetApiResource() );
}

void GraphicsContext::SetPipeline( SharedRef<Pipeline> pipeline ) {
	glBindProgramPipeline( pipeline->GetApiResource() );
}

void GraphicsContext::SetSampler( uint32_t unit, SharedRef<Sampler> sampler ) {
	glBindSampler( unit, sampler->GetApiResource() );
}


void GraphicsContext::SetTextureUnit( uint32_t unit, SharedRef<Texture2D> texture ) {
	glBindTextureUnit( unit, texture->GetApiResource() );
}

void GraphicsContext::SetViewport( int x, int y, int width,
									 int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

}  // namespace Octave::Impl
