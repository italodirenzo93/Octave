#include "Renderer.hpp"

#include <glad/glad.h>

#include <sstream>

#include "Config.hpp"

using namespace std;

namespace Octave {

Renderer::Renderer() noexcept {
	const Config& config = Config::Instance();

	// Always enabled
	glEnable( GL_DEPTH_TEST );
	glClearDepth( 1.0 );

	if ( config.IsCullFaceEnabled() ) {
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CW );
	}
}

void Renderer::Clear( bool color, bool depth, float r, float g, float b,
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

void Renderer::Draw( const VertexBuffer& vbo ) const noexcept {
	glBindVertexArray( vbo.vao_ );

	glDrawArrays( GL_TRIANGLES, 0, static_cast<int>( vbo.GetVertexCount() ) );

	glBindVertexArray( 0 );
}

void Renderer::DrawIndexed( const VertexBuffer& vbo,
							const IndexBuffer& ibo ) const noexcept {
	glBindVertexArray( vbo.vao_ );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo.id_ );

	glDrawElements( GL_TRIANGLES, static_cast<int>( ibo.GetElementCount() ),
					GL_UNSIGNED_INT, nullptr );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );
}

std::string Renderer::GetDescription() const noexcept {
	ostringstream oss;

	// Print OpenGL context information
	oss << "OpenGL Context Version: " << glGetString( GL_VERSION ) << endl
		<< "GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION )
		<< endl
		<< "GPU Vendor: " << glGetString( GL_VENDOR ) << endl
		<< "GPU Model: " << glGetString( GL_RENDERER ) << endl;

	return oss.str();
}

void Renderer::SetDepthTestEnabled( bool enabled ) noexcept {
	if ( enabled ) {
		glEnable( GL_DEPTH_TEST );
	} else {
		glDisable( GL_DEPTH_TEST );
	}
}

void Renderer::SetShader( const Shader& shader ) noexcept {
	glUseProgram( shader.id_ );
}

void Renderer::SetViewport( int x, int y, int width, int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

}  // namespace Octave
