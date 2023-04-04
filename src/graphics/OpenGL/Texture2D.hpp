#ifndef OCTAVE_GRAPHICS_OPENGL_TEXTURE2D_HPP
#define OCTAVE_GRAPHICS_OPENGL_TEXTURE2D_HPP

#include "graphics/Structs.hpp"

#include <glad/glad.h>

namespace Octave {

struct Texture2D {
	GLuint handle_;
	TextureDescription2D desc_;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return handle_; }
	void SetApiResource( GLuint resource ) noexcept { handle_ = resource; }

	void SetData( TextureFormat format, int mip_level, int xoffset, int yoffset,
				  uint32_t width, uint32_t height, const void* data );
};

}  // namespace Octave

#endif
