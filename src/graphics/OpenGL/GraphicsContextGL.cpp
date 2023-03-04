#include "pch/pch.hpp"
#include "GraphicsContextGL.hpp"

#include <glad/glad.h>

#include <array>
#include <sstream>

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

	// glBindVertexArray( ( dynamic_cast<const VertexBufferGL&>( vbo )
	// ).GetVaoId() );

	glDrawArrays( GL_TRIANGLES, static_cast<GLint>( offset ),
				  static_cast<GLsizei>( vertex_count ) );

	// glBindVertexArray( 0 );

	// glUseProgram( 0 );
}

void GraphicsContextGL::DrawIndexed( size_t index_count, size_t offset,
									 size_t base_vertex ) const noexcept {
	// glUseProgram( shader.id_ );
	// glBindVertexArray( ( dynamic_cast<const VertexBufferGL&>( vbo )
	// ).GetVaoId() ); glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, (
	// dynamic_cast<const IndexBufferGL&>( ibo ) ).GetId() );

	// glDrawElements( GL_TRIANGLES, static_cast<GLsizei>( index_count ),
	// 				GL_UNSIGNED_INT, reinterpret_cast<const void*>( offset ) );

	// TODO: check the currently bound index buffer for the **end** parameter

	glDrawRangeElementsBaseVertex( GL_TRIANGLES, static_cast<GLuint>( offset ),
								   static_cast<GLuint>( offset + index_count ), static_cast<GLsizei>( index_count ),
								   GL_UNSIGNED_SHORT, nullptr,
								   static_cast<GLint>( base_vertex ) );

	// glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	// glBindVertexArray( 0 );
	// glUseProgram( 0 );
}

std::string GraphicsContextGL::GetDescription() const noexcept {
	ostringstream oss;

	// Print OpenGL context information
	oss << "OpenGL Context Version: " << glGetString( GL_VERSION ) << endl
		<< "GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION )
		<< endl
		<< "GPU Vendor: " << glGetString( GL_VENDOR ) << endl
		<< "GPU Model: " << glGetString( GL_RENDERER ) << endl;

	return oss.str();
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

void GraphicsContextGL::SetViewport( int x, int y, int width,
									 int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

}  // namespace Octave::Impl
