#include "pch/pch.hpp"
#include "TextureGL.hpp"

#include <glad/glad.h>

namespace Octave::Impl {

static GLint TextureFormatToGLFormat( TextureFormat format ) noexcept {
	switch ( format ) {
		case TextureFormat::kBgra:
			return GL_BGRA;
		case TextureFormat::kRgba:
			return GL_RGBA;
		case TextureFormat::kRgb:
			return GL_RGB;
		default:
			assert( false );
	}
}

static TextureFormat GLFormatToOctaveFormat( GLint format ) noexcept {
	switch ( format ) {
		case GL_BGRA:
			return TextureFormat::kBgra;
		case GL_RGBA:
			return TextureFormat::kRgba;
		case GL_RGB:
			return TextureFormat::kRgb;
		default:
			assert( false );
	}
}

TextureGL::TextureGL() noexcept {
	glGenTextures( 1, &id_ );
}

TextureGL::~TextureGL() noexcept {
	glDeleteTextures( 1, &id_ );
}

TextureFormat TextureGL::GetFormat() const noexcept {
	GLint format;
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_INTERNAL_FORMAT, &format );

	return GLFormatToOctaveFormat( format );
}

void TextureGL::SetData( TextureFormat format, int width, int height,
						 bool use_mipmaps, const void* pixels ) {
	const auto gl_format = TextureFormatToGLFormat( format );

	glBindTexture( GL_TEXTURE_2D, id_ );

	glTexImage2D( GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format,
				  GL_UNSIGNED_BYTE, pixels );

	if ( use_mipmaps ) {
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );
}

}  // namespace Octave::Impl
