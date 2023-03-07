#include "pch/pch.hpp"
#include "Pipeline.hpp"

#include "core/Log.hpp"

namespace Octave {

Pipeline::Pipeline() {
	glGenProgramPipelines( 1, &id_ );
}

Pipeline::~Pipeline() noexcept {
	glDeleteProgramPipelines( 1, &id_ );
}

void Pipeline::SetVertexShader( SharedRef<Shader> vertex_shader ) noexcept {
	glUseProgramStages( id_, GL_VERTEX_SHADER_BIT,
						vertex_shader->GetApiResource() );

	vertex_shader_ = std::move( vertex_shader );
}

void Pipeline::SetVertexUniformBuffer( SharedRef<Buffer> ubo, uint32_t index, uint32_t binding ) noexcept {
	assert( binding < GL_MAX_UNIFORM_BUFFER_BINDINGS );

	glUniformBlockBinding( vertex_shader_->GetApiResource(), index, binding );

	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo->GetApiResource() );

	vs_ubos_[binding] = std::move( ubo );
}


void Pipeline::SetFragmentShader( SharedRef<Shader> fragment_shader ) noexcept {
	glUseProgramStages( id_, GL_FRAGMENT_SHADER_BIT,
						fragment_shader->GetApiResource() );

	fragment_shader_ = std::move( fragment_shader );
}

void Pipeline::SetFragmentUniformBuffer( SharedRef<Buffer> ubo, uint32_t index, uint32_t binding ) noexcept {
	assert( binding < GL_MAX_UNIFORM_BUFFER_BINDINGS );

	glUniformBlockBinding( fragment_shader_->GetApiResource(), index, binding );

	glBindBufferBase( GL_UNIFORM_BUFFER, binding, ubo->GetApiResource() );

	fs_ubos_[binding] = std::move( ubo );
}

}  // namespace Octave
