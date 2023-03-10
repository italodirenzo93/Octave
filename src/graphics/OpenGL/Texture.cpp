#include "pch/pch.hpp"
#include "Texture.hpp"

namespace Octave {

static GLint TextureFormatToGLType( TextureFormat format ) noexcept {
	switch ( format ) {
		case TextureFormat::Rgb:
			return GL_RGB;
		case TextureFormat::Rgba:
			return GL_RGBA;
		default:
			assert( false );
	}
}

Texture::Texture( const TextureDescription2D& desc, const void* data ) {
	glGenTextures( 1, &id_ );

	glBindTexture( GL_TEXTURE_2D, id_ );

	glTexImage2D( GL_TEXTURE_2D, static_cast<GLint>( desc.mip_levels ), TextureFormatToGLType( desc.format ),
				  static_cast<GLsizei>( desc.width ),
				  static_cast<GLsizei>( desc.height ), 0, TextureFormatToGLType( desc.format ),
				  GL_UNSIGNED_BYTE, data );
	//glTexImage2DMultisample( GL_TEXTURE_2D, 1, GL_RGBA, static_cast<GLsizei>( desc.width ), static_cast<GLsizei>( desc.height ),
	//						 GL_FALSE );
	glGenerateMipmap( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

Texture::~Texture() noexcept {
	glDeleteTextures( 1, &id_ );
}

}
