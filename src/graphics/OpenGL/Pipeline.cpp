#include "pch/pch.hpp"
#include "Pipeline.hpp"

#include "core/Log.hpp"

namespace Octave {

Pipeline::Pipeline() {
	glCreateProgramPipelines( 1, &id_ );
}

Pipeline::~Pipeline() noexcept {
	glDeleteProgramPipelines( 1, &id_ );
}

void Pipeline::SetVertexShader( SharedRef<Shader> vertex_shader ) noexcept {
	glUseProgramStages( id_, GL_VERTEX_SHADER_BIT,
						vertex_shader->GetApiResource() );

	vertex_shader_ = std::move( vertex_shader );
}

void Pipeline::SetFragmentShader( SharedRef<Shader> fragment_shader ) noexcept {
	glUseProgramStages( id_, GL_FRAGMENT_SHADER_BIT,
						fragment_shader->GetApiResource() );

	fragment_shader_ = std::move( fragment_shader );
}

}  // namespace Octave
