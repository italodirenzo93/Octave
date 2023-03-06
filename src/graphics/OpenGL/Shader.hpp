#ifndef OCTAVE_GRAPHICS_OPENGL_SHADER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SHADER_HPP

#include <glad/glad.h>

#include "graphics/Structs.hpp"

namespace Octave {

class Shader {
public:
	Shader();
	~Shader() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

private:
	GLuint id_ = 0;
};

}

#endif
