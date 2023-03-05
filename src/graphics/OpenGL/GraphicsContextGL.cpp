#include "pch/pch.hpp"
#include "GraphicsContextGL.hpp"

#include <glad/glad.h>

#include <array>

#include "Config.hpp"
#include "BufferGL.hpp"

using namespace std;

namespace Octave::Impl {

GraphicsContextGL::GraphicsContextGL() {
	const Config& config = Config::Instance();

	// Always enabled
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glClearDepth( 1.0 );

	if ( config.IsCullFaceEnabled() ) {
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CW );
	}
}

GraphicsContextGL::~GraphicsContextGL() noexcept {
}

void GraphicsContextGL::Clear( bool color, bool depth, float r, float g,
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

void GraphicsContextGL::Draw( size_t vertex_count,
							  size_t offset ) const noexcept {
	// glUseProgram( shader.id_ );

	assert( vbo_ != nullptr );
	assert( vao_ != nullptr );

	vbo_->Bind();
	vao_->Bind();

	uint32_t attr_index = 0;
	for ( const auto& attr : *vao_ ) {
		glVertexAttribPointer(
			attr.index, attr.size, attr.type, attr.normalized,
			static_cast<GLsizei>( vertex_stride_ ),
			reinterpret_cast<const void*>( attr_index++ * sizeof( float ) ) );
	}

	vbo_->Unbind();

	glDrawArrays( GL_TRIANGLES, static_cast<GLint>( offset ),
				  static_cast<GLsizei>( vertex_count ) );

	vao_->Unbind();

	// glUseProgram( 0 );
}

void GraphicsContextGL::DrawIndexed( size_t index_count, size_t offset,
									 size_t base_vertex ) const noexcept {
	// glUseProgram( shader.id_ );

	assert( vbo_ != nullptr );
	assert( vao_ != nullptr );
	assert( ibo_ != nullptr );

	vbo_->Bind();
	vao_->Bind();

	uint32_t attr_index = 0;
	for ( const auto& attr : *vao_ ) {
		glVertexAttribPointer(
			attr.index, attr.size, attr.type, attr.normalized,
			static_cast<GLsizei>( vertex_stride_ ),
			reinterpret_cast<const void*>( attr_index++ * sizeof( float ) ) );
	}

	vbo_->Unbind();

	ibo_->Bind();

	glDrawRangeElementsBaseVertex( GL_TRIANGLES, static_cast<GLuint>( offset ),
								   static_cast<GLuint>( ibo_->GetSize() ),
								   static_cast<GLsizei>( index_count ),
								   GL_UNSIGNED_SHORT, nullptr,
								   static_cast<GLint>( base_vertex ) );

	ibo_->Unbind();
	vao_->Unbind();

	// glUseProgram( 0 );
}

std::array<int, 4> GraphicsContextGL::GetViewport() const noexcept {
	std::array<int, 4> vp;
	glGetIntegerv( GL_VIEWPORT, vp.data() );
	return vp;
}

void GraphicsContextGL::SetDepthTestEnabled( bool enabled ) noexcept {
	if ( enabled ) {
		glEnable( GL_DEPTH_TEST );
	} else {
		glDisable( GL_DEPTH_TEST );
	}
}

void GraphicsContextGL::SetVertexBuffer( SharedRef<Buffer> vertex_buffer,
										 size_t stride ) {
	vbo_ = dynamic_pointer_cast<BufferGL>( vertex_buffer );
	vertex_stride_ = static_cast<GLuint>( stride );
}

void GraphicsContextGL::SetIndexBuffer( SharedRef<Buffer> index_buffer ) {
	ibo_ = dynamic_pointer_cast<BufferGL>( index_buffer );
}

void GraphicsContextGL::SetVertexLayout( SharedRef<VertexArrayLayout> layout ) {
	vao_ = dynamic_pointer_cast<VertexArrayLayoutGL>( layout );
}

void GraphicsContextGL::SetViewport( int x, int y, int width,
									 int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

}  // namespace Octave::Impl
