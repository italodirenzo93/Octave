#ifndef OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

class Sampler {
public:
	explicit Sampler( const SamplerDescription& desc );
	~Sampler() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }
	[[nodiscard]] SamplerDescription GetDescription() const noexcept {
		return desc_;
	}

private:
	GLuint id_;
	SamplerDescription desc_;
};

}

#endif
