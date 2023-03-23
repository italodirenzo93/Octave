#include "pch/pch.hpp"
#include "graphics/GraphicsContext.hpp"

#include <glad/glad.h>

#include <array>

#include "Config.hpp"

using namespace std;

namespace Octave {

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

void GraphicsContext::SetVertexArray( SharedRef<VertexArray> vao ) {
	glBindVertexArray( vao->GetApiResource() );
}

void GraphicsContext::SetPipeline( SharedRef<Pipeline> pipeline ) {
	glBindProgramPipeline( pipeline->GetApiResource() );
}

void GraphicsContext::SetSampler( uint32_t unit, SharedRef<Sampler> sampler ) {
	glBindSampler( unit, sampler->GetApiResource() );
}

void GraphicsContext::SetTexture( uint32_t unit, SharedRef<Texture2D> texture ) {
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

void GraphicsContext::Signal( SharedRef<Fence> fence ) {
	if ( fence->sync_ ) {
		glDeleteSync( fence->sync_ );
	}

	fence->sync_ = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
}

void GraphicsContext::Wait( SharedRef<Fence> fence, uint64_t timeout ) {
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
