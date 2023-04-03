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

static GLint max_indices = 0;

GraphicsContext::GraphicsContext() {
	if ( max_indices == 0 ) {
		glGetIntegerv( GL_MAX_ELEMENTS_INDICES, &max_indices );
	}

	Reset();
}

GraphicsContext::~GraphicsContext() noexcept {
}

void GraphicsContext::Reset() noexcept {
	glBindVertexArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glUseProgram( 0 );

	GLint n_texture_units;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &n_texture_units );

	for ( int i = 0; i < n_texture_units; i++ ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glBindSampler( i, 0 );
	}
	glActiveTexture( GL_TEXTURE0 );

	// Enable depth by default
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0 );

	// Default culling options
	glCullFace( GL_BACK );
	glFrontFace( GL_CW );

	const Config& config = Config::Instance();
	if ( config.IsCullFaceEnabled() ) {
		glEnable( GL_CULL_FACE );
	}
}

void GraphicsContext::Clear( bool color, bool depth, float r, float g, float b,
							 float a ) const noexcept {
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

void GraphicsContext::SetVertexBuffer( SharedRef<Buffer> vbo,
									   SharedRef<VertexArray> vao ) {
	glBindVertexArray( vao->GetApiResource() );

	// Define vertex data layout
	glBindBuffer( GL_ARRAY_BUFFER, vbo->GetApiResource() );

	uint32_t offset = 0;

	for ( const auto& attr : *vao ) {
		glVertexAttribPointer(
			AttrNameToIndex( attr.name ), static_cast<GLint>( attr.size ),
			AttrTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ), vbo->GetStride(),
			reinterpret_cast<const void*>( offset ) );
		glEnableVertexAttribArray( AttrNameToIndex( attr.name ) );

		offset += attr.size * AttrTypeSize( attr.type );
	}
}

void GraphicsContext::SetIndexBuffer( SharedRef<Buffer> ibo ) {
	if ( ibo == nullptr ) {
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	} else {
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo->GetApiResource() );
	}
}

void GraphicsContext::SetProgram( SharedRef<Program> program ) {
	if ( program == nullptr ) {
		glUseProgram( 0 );
	} else {
		glUseProgram( program->GetApiResource() );
	}
}

void GraphicsContext::SetSampler( uint32_t unit, SharedRef<Sampler> sampler ) {
	if ( sampler == nullptr ) {
		glBindSampler( unit, 0 );
	} else {
		glBindSampler( unit, sampler->GetApiResource() );
	}
}


void GraphicsContext::SetTexture( uint32_t unit,
								  SharedRef<Texture2D> texture ) {
	glActiveTexture( GL_TEXTURE0 + unit );
	if ( texture == nullptr ) {
		glBindTexture( GL_TEXTURE_2D, 0 );
	} else {
		glBindTexture( GL_TEXTURE_2D, texture->GetApiResource() );
	}
}

void GraphicsContext::SetViewport( int x, int y, int width,
								   int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

void GraphicsContext::Signal( SharedRef<Fence> fence ) {
	assert( fence != nullptr );

	if ( fence->sync_ ) {
		glDeleteSync( fence->sync_ );
	}

	fence->sync_ = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
}

void GraphicsContext::Wait( SharedRef<Fence> fence, uint64_t timeout ) {
	assert( fence != nullptr );

	if ( fence->sync_ ) {
		GLenum wait_return = 0;
		while ( wait_return != GL_ALREADY_SIGNALED &&
				wait_return != GL_CONDITION_SATISFIED ) {
			wait_return =
				glClientWaitSync( fence->sync_, GL_SYNC_FLUSH_COMMANDS_BIT,
								  timeout == 0 ? GL_TIMEOUT_IGNORED : timeout );
		}
	}
}

}  // namespace Octave
