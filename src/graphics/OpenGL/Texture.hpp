#ifndef OCTAVE_GRAPHICS_OPENGL_TEXTURE_HPP
#define OCTAVE_GRAPHICS_OPENGL_TEXTURE_HPP

#include <glad/glad.h>

namespace Octave {

class Texture {
public:
	Texture();
	~Texture() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return 0; }

private:
	GLuint id_ = 0;
};

}

#endif
