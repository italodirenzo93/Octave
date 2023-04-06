#include "pch/pch.hpp"
#include "GraphicsContext.hpp"

#include <glad/glad.h>

using namespace std;

namespace Octave::OpenGL {

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
			return 0;
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

GraphicsContext::GraphicsContext() noexcept : m_Vao( 0 ) {
	// Enable depth by default
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0 );

	// Default culling options
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &m_MaxTextures );
	m_Samplers.reserve( m_MaxTextures );
	m_Textures.reserve( m_MaxTextures );

	glGetIntegerv( GL_MAX_ELEMENTS_INDICES, &m_MaxIndices );

	Reset();
}

void GraphicsContext::Reset() noexcept {
	m_Vbo = 0;
	m_Ibo = 0;
	m_Program = 0;
	m_VertexStride = 0;
	m_VertexLayout.clear();
	std::fill( m_Samplers.begin(), m_Samplers.end(), 0 );
	std::fill( m_Textures.begin(), m_Textures.end(), 0 );
}

void GraphicsContext::Clear( bool color, bool depth, float r, float g, float b,
							 float a ) noexcept {
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

void GraphicsContext::Draw( size_t vertex_count, size_t offset ) noexcept {
	PrepareToDraw();
	glDrawArrays( GL_TRIANGLES, static_cast<GLint>( offset ),
				  static_cast<GLsizei>( vertex_count ) );
}

void GraphicsContext::DrawIndexed( size_t index_count, size_t offset,
								   size_t base_vertex ) noexcept {
	PrepareToDraw();
	glDrawRangeElementsBaseVertex(
		GL_TRIANGLES, static_cast<GLuint>( offset ), m_MaxIndices,
		static_cast<GLsizei>( index_count ), GL_UNSIGNED_SHORT, nullptr,
		static_cast<GLint>( base_vertex ) );
}

std::array<int, 4> GraphicsContext::GetViewport() const noexcept {
	std::array<int, 4> vp;
	glGetIntegerv( GL_VIEWPORT, vp.data() );
	return vp;
}

void GraphicsContext::SetVertexBuffer( const Buffer& vbo ) {
	const GLuint resource = vbo.GetApiResource();
	if ( resource != 0 ) {
		assert( glIsBuffer( resource ) );
	}

	m_Vbo = resource;
	m_VertexStride = vbo.GetStride();
}

void GraphicsContext::SetIndexBuffer( const Buffer& ibo ) {
	const GLuint resource = ibo.GetApiResource();
	if ( resource != 0 ) {
		assert( glIsBuffer( resource ) );
	}

	m_Ibo = resource;
}

void GraphicsContext::SetProgram( const Program& program ) {
	const GLuint resource = program.GetApiResource();
	if ( resource != 0 ) {
		assert( glIsProgram( resource ) );
	}

	m_Program = resource;
}

void GraphicsContext::SetSampler( uint32_t unit, const Sampler& sampler ) {
	assert( unit < m_MaxTextures );

	const GLuint resource = sampler.GetApiResource();
	if ( resource != 0 ) {
		assert( glIsSampler( resource ) );
	}

	m_Samplers.emplace( m_Samplers.begin() + unit, resource );
}

void GraphicsContext::SetTexture( uint32_t unit, const Texture2D& texture ) {
	assert( unit < m_MaxTextures );

	const GLuint resource = texture.GetApiResource();
	if ( resource != 0 ) {
		assert( glIsTexture( resource ) );
	}

	m_Textures.emplace( m_Textures.begin() + unit, resource );
}

void GraphicsContext::SetVertexLayout( const VertexLayout& layout ) {
	m_VertexLayout = layout;
}

void GraphicsContext::SetViewport( int x, int y, int width, int height ) {
	assert( x >= 0 && y >= 0 && width > 0 && height > 0 );
	glViewport( x, y, width, height );
}

void GraphicsContext::PrepareToDraw() {
	// Bind vertex array
	glBindVertexArray( m_Vao );

	// Bind data buffers
	glBindBuffer( GL_ARRAY_BUFFER, m_Vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Ibo );

	// Set up vertex data
	uint32_t offset = 0;
	for ( const auto& attr : m_VertexLayout ) {
		glVertexAttribPointer(
			AttrNameToIndex( attr.name ), static_cast<GLint>( attr.size ),
			AttrTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ), m_VertexStride,
			reinterpret_cast<const void*>( offset ) );
		glEnableVertexAttribArray( AttrNameToIndex( attr.name ) );

		offset += attr.size * AttrTypeSize( attr.type );
	}

	// Bind textures
	for ( int i = 0; i < m_MaxTextures; i++ ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, m_Textures[i] );
	}

	// Bind samplers
	for ( int i = 0; i < m_MaxTextures; i++ ) {
		glBindSampler( i, m_Samplers[i] );
	}

	// Use shader program
	glUseProgram( m_Program );
}

}  // namespace Octave::OpenGL
