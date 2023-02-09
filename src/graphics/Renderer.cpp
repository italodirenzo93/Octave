#include "Renderer.hpp"

#include <sstream>

#include "Config.hpp"
#include "ShaderManager.hpp"

using namespace std;

namespace octave::graphics {

static inline GLenum PrimitiveToGLType( PrimitiveType type ) {
	switch ( type ) {
		case PrimitiveType::kTriangleList:
			return GL_TRIANGLES;
		case PrimitiveType::kTriangleStrip:
			return GL_TRIANGLE_STRIP;
		case PrimitiveType::kTriangleFan:
			return GL_TRIANGLE_FAN;
	}

	return 0;
}

Renderer::Renderer( const Window& window ) noexcept : window_( window ) {
	const Config& config = Config::Instance();

	// Always enabled
	glEnable( GL_DEPTH_TEST );
	glClearDepth( 1.0f );

	if ( config.IsCullFaceEnabled() ) {
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( GL_CW );
	}

	// Shader pre-cache
	if ( config.GetPreloadShaders() ) {
		ShaderManager::Instance().PreloadShaders();
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
		glClearDepth( 1.0f );
	}

	glClear( clear_flags );
}

void Renderer::Present() const noexcept {
	glfwSwapBuffers( window_.handle_ );
}

void Renderer::DrawPrimitives( PrimitiveType type,
							   const VertexBuffer& vbo ) const noexcept {
	GL_CALL( glBindVertexArray( vbo.vao_ ) );

	GL_CALL( glDrawArrays( PrimitiveToGLType( type ), 0,
						   static_cast<int>( vbo.GetVertexCount() ) ) );

	glBindVertexArray( 0 );
}

void Renderer::DrawIndexedPrimitives( PrimitiveType type,
									  const VertexBuffer& vbo,
									  const IndexBuffer& ibo ) const noexcept {
	GL_CALL( glBindVertexArray( vbo.vao_ ) );
	GL_CALL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo.id_ ) );

	GL_CALL( glDrawElements( PrimitiveToGLType( type ),
							 static_cast<int>( ibo.GetElementCount() ),
							 GL_UNSIGNED_INT, nullptr ) );

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

void Renderer::GetFramebufferSize( int& width, int& height ) const noexcept {
	glfwGetFramebufferSize( window_.handle_, &width, &height );
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

}  // namespace octave::graphics
