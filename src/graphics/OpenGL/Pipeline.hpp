#ifndef OCTAVE_GRAPHICS_OPENGL_PIPELINE_HPP
#define OCTAVE_GRAPHICS_OPENGL_PIPELINE_HPP

#include "core/Types.hpp"
#include "Shader.hpp"

namespace Octave {

class Pipeline {
public:
	Pipeline();
	~Pipeline() noexcept;

	[[nodiscard]] uint32_t GetApiResource() const noexcept { return id_; }

	void SetVertexShader( SharedRef<Shader> vertex_shader ) noexcept;
	void SetFragmentShader( SharedRef<Shader> fragment_shader ) noexcept;

private:
	GLuint id_ = 0;
	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
};

}

#endif
