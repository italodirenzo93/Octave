#ifndef OCTAVE_GRAPHICS_OPENGL_PROGRAMGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_PROGRAMGL_HPP

#include "graphics/Program.hpp"

namespace Octave::Impl {

class ShaderGL {
public:
	[[nodiscard]] uint32_t GetId() const noexcept { return 0; }
};

class ProgramGL : public Program {
public:
	ProgramGL();
	virtual ~ProgramGL() noexcept override;

	[[nodiscard]] uint32_t GetId() const noexcept { return id_; }

	void Use() const noexcept;
	void Unuse() const noexcept;
	void Link();

	void SetVertexShader( SharedRef<Shader> vertex_shader ) override;
	void SetPixelShader( SharedRef<Shader> pixel_shader ) override;

private:
	uint32_t id_ = 0;
	SharedRef<ShaderGL> vertex_shader_;
	SharedRef<ShaderGL> fragment_shader_;
};

}

#endif
