#ifndef OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP
#define OCTAVE_GRAPHICS_OPENGL_SAMPLER_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

class Sampler {
public:
	explicit Sampler( const SamplerDescription& desc ) noexcept
		: handle_( 0 ), desc_( desc ) {}

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }

private:
	GLuint handle_;
	SamplerDescription desc_;
};

}  // namespace Octave

#endif
