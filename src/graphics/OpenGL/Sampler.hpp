#ifndef OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

struct Sampler {
	GLuint handle_;
	SamplerDescription desc_;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }
};

}

#endif
