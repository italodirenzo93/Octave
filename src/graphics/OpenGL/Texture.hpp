#ifndef OCTAVE_GRAPHICS_OPENGL_TEXTURE_HPP
#define OCTAVE_GRAPHICS_OPENGL_TEXTURE_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

class Texture {
public:
	Texture( const TextureDescription2D& desc, const void* data );
	~Texture() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

private:
	GLuint id_ = 0;
};

}

#endif
