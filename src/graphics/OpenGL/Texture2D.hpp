#ifndef OCTAVE_GRAPHICS_OPENGL_TEXTURE2D_HPP
#define OCTAVE_GRAPHICS_OPENGL_TEXTURE2D_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

class Texture2D {
public:
	Texture2D( const TextureDescription2D& desc );
	~Texture2D() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return id_; }

	void SetData( TextureFormat format, int mip_level, int xoffset, int yoffset,
				  uint32_t width, uint32_t height, const void* data );

private:
	GLuint id_;
	TextureDescription2D desc_;
};

}

#endif
