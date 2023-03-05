#ifndef OCTAVE_GRAPHICS_PROGRAM_HPP
#define OCTAVE_GRAPHICS_PROGRAM_HPP

#include "core/Types.hpp"
#include "Shader.hpp"

namespace Octave {

class Program {
public:
	virtual ~Program() noexcept = default;

	virtual void SetVertexShader( SharedRef<Shader> vertex_shader ) = 0;
	virtual void SetPixelShader( SharedRef<Shader> pixel_shader ) = 0;
};

}

#endif
