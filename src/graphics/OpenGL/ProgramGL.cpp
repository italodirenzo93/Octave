#include "pch/pch.hpp"
#include "ProgramGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

ProgramGL::ProgramGL() {
	id_ = glCreateProgram();
}

ProgramGL::~ProgramGL() noexcept {
	glDeleteProgram( id_ );
}

void ProgramGL::Use() const noexcept {
	glUseProgram( id_ );
}

void ProgramGL::Unuse() const noexcept {
#ifdef OGT_DEBUG
	glUseProgram( 0 );
#endif
}

void ProgramGL::Link() {
	assert( vertex_shader_ != nullptr );
	assert( fragment_shader_ != nullptr );

	glAttachShader( id_, vertex_shader_->GetId() );
	glAttachShader( id_, fragment_shader_->GetId() );

	glLinkProgram( id_ );

	glDetachShader( id_, vertex_shader_->GetId() );
	glDetachShader( id_, fragment_shader_->GetId() );

	GLint status = GL_NO_ERROR;
	glGetProgramiv( id_, GL_LINK_STATUS, &status );

	if ( status != GL_NO_ERROR ) {
		std::string msg;
		msg.reserve( 512 );

		glGetProgramInfoLog( id_, static_cast<GLsizei>( msg.size() ), nullptr,
							 msg.data() );

		throw Exception( msg );
	}

	// Link succeeded, released references
	vertex_shader_ = nullptr;
	fragment_shader_ = nullptr;
}

void ProgramGL::SetVertexShader( SharedRef<Shader> vertex_shader ) {
	vertex_shader_ = std::dynamic_pointer_cast<ShaderGL>( vertex_shader );
}

void ProgramGL::SetPixelShader( SharedRef<Shader> pixel_shader ) {
	fragment_shader_ = std::dynamic_pointer_cast<ShaderGL>( pixel_shader );
}

}  // namespace Octave::Impl
