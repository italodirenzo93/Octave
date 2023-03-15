#ifndef OCTAVE_GRAPHICS_OPENGL_PIPELINE_HPP
#define OCTAVE_GRAPHICS_OPENGL_PIPELINE_HPP

#include <array>

#include "core/Types.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace Octave {

class Pipeline {
public:
	using UniformBufferArray = std::array<SharedRef<Buffer>, GL_MAX_UNIFORM_BUFFER_BINDINGS>;

	Pipeline();
	~Pipeline() noexcept;

	[[nodiscard]] uint32_t GetApiResource() const noexcept { return id_; }

	void SetVertexShader( SharedRef<Shader> vertex_shader ) noexcept;
	void SetVertexUniformBuffer( SharedRef<Buffer> ubo, uint32_t index, uint32_t binding ) noexcept;

	void SetFragmentShader( SharedRef<Shader> fragment_shader ) noexcept;
	void SetFragmentUniformBuffer( SharedRef<Buffer> ubo, uint32_t index, uint32_t binding ) noexcept;

private:
	GLuint id_ = 0;

	SharedRef<Shader> vertex_shader_;
	SharedRef<Shader> fragment_shader_;
};

}

#endif
