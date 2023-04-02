#include "pch/pch.hpp"
#include "Texture2D.hpp"

namespace Octave {

static GLint TextureFormatToGLTypeInternal( TextureFormat format ) noexcept {
	switch ( format ) {
		case TextureFormat::Rgb:
			return GL_RGB32F;
		case TextureFormat::Rgba:
			return GL_RGBA32F;
		default:
			assert( false );
			return 0;
	}
}

static GLint TextureFormatToGLType( TextureFormat format ) noexcept {
	switch ( format ) {
		case TextureFormat::Rgb:
			return GL_RGB;
		case TextureFormat::Rgba:
			return GL_RGBA;
		default:
			assert( false );
			return 0;
	}
}

Texture2D::Texture2D( const TextureDescription2D& desc )
	: id_( 0 ), desc_( desc ) {
	glGenTextures( 1, &id_ );

	glBindTexture( GL_TEXTURE_2D, id_ );

	if ( GLAD_GL_ARB_texture_storage ) {
		glTexStorage2D( GL_TEXTURE_2D, static_cast<GLsizei>( desc.mip_levels ),
						TextureFormatToGLTypeInternal( desc.format ),
						static_cast<GLsizei>( desc.width ),
						static_cast<GLsizei>( desc.height ) );
	} else {
		glTexImage2D(
			GL_TEXTURE_2D, 0, TextureFormatToGLTypeInternal( desc.format ),
			static_cast<GLsizei>( desc.width ),
			static_cast<GLsizei>( desc.height ), 0,
			TextureFormatToGLType( desc.format ), GL_UNSIGNED_BYTE, nullptr );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );
}

Texture2D::~Texture2D() noexcept {
	glDeleteTextures( 1, &id_ );
}

void Texture2D::SetData( TextureFormat format, int mip_level, int xoffset,
						 int yoffset, uint32_t width, uint32_t height,
						 const void* data ) {
	glBindTexture( GL_TEXTURE_2D, id_ );

	glTexSubImage2D( GL_TEXTURE_2D, mip_level, xoffset, yoffset,
					 static_cast<GLsizei>( width ),
					 static_cast<GLsizei>( height ),
					 TextureFormatToGLType( format ), GL_UNSIGNED_BYTE, data );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::GenerateMipmap() {
	glBindTexture( GL_TEXTURE_2D, id_ );

	glGenerateMipmap( GL_TEXTURE_2D );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

}  // namespace Octave
