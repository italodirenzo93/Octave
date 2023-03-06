#include "pch/pch.hpp"
#include "GraphicsContextGL.hpp"

#include <glad/glad.h>

#include <array>

#include "Config.hpp"

using namespace std;

namespace Octave::Impl {

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
	assert( vao_ != nullptr );

	glBindProgramPipeline( pipeline_->GetApiResource() );
	glBindVertexArray( vao_->GetApiResource() );

	glDrawArrays( GL_TRIANGLES, static_cast<GLint>( offset ),
				  static_cast<GLsizei>( vertex_count ) );
}

void GraphicsContextGL::DrawIndexed( size_t index_count, size_t offset,
									 size_t base_vertex ) const noexcept {
	assert( vao_ != nullptr );
	assert( ibo_ != nullptr );

	glBindProgramPipeline( pipeline_->GetApiResource() );
	glBindVertexArray( vao_->GetApiResource() );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_->GetApiResource() );

	glDrawRangeElementsBaseVertex( GL_TRIANGLES, static_cast<GLuint>( offset ),
								   static_cast<GLuint>( ibo_->GetSize() ),
								   static_cast<GLsizei>( index_count ),
								   GL_UNSIGNED_SHORT, nullptr,
								   static_cast<GLint>( base_vertex ) );
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
										 SharedRef<VertexArray> layout ) {
	// Define vertex data layout
	glBindVertexArray( layout->GetApiResource() );
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer->GetApiResource() );

	uint32_t attr_index = 0;
	// TODO: map the attribute type to the correct number of bytes in offset
	// (currently hardcoded to sizeof(float))
	for ( const auto& attr : *layout ) {
		glVertexAttribPointer(
			AttrNameToIndex( attr.name ), static_cast<GLint>( attr.size ),
			AttrTypeToGLType( attr.type ),
			static_cast<GLboolean>( attr.normalized ),
			static_cast<GLsizei>( vertex_buffer->GetStride() ),
			reinterpret_cast<const void*>( attr_index++ * sizeof( float ) ) );
		glEnableVertexAttribArray( AttrNameToIndex( attr.name ) );
	}

	// Save references for draw time
	vao_ = std::move( layout );
}

void GraphicsContextGL::SetIndexBuffer( SharedRef<Buffer> index_buffer ) {
	ibo_ = std::move( index_buffer );
}

void GraphicsContextGL::SetPipeline( SharedRef<Pipeline> pipeline ) {
	pipeline_ = std::move( pipeline );
}

// void GraphicsContextGL::SetVertexShader( SharedRef<Shader> vertex_shader ) {
// 	glUseProgramStages( pipeline_id_, GL_VERTEX_SHADER_BIT,
// vertex_shader->GetApiResource() );
// }

// void GraphicsContextGL::SetFragmentShader( SharedRef<Shader> fragment_shader
// ) { 	glUseProgramStages( pipeline_id_, GL_FRAGMENT_SHADER_BIT,
// fragment_shader->GetApiResource() );
// }

void GraphicsContextGL::SetViewport( int x, int y, int width,
									 int height ) noexcept {
	assert( x >= 0 );
	assert( y >= 0 );
	assert( width > 0 );
	assert( height > 0 );

	glViewport( x, y, width, height );
}

}  // namespace Octave::Impl
