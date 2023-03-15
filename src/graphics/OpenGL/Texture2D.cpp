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
	}
}

Texture2D::Texture2D( const TextureDescription2D& desc ) : id_( 0 ), desc_( desc ) {
	glCreateTextures( GL_TEXTURE_2D, 1, &id_ );
	glTextureStorage2D( id_, static_cast<GLsizei>( desc.mip_levels ),
						TextureFormatToGLTypeInternal( desc.format ),
						static_cast<GLsizei>( desc.width ),
						static_cast<GLsizei>( desc.height ) );
}

Texture2D::~Texture2D() noexcept {
	glDeleteTextures( 1, &id_ );
}

void Texture2D::SetData( TextureFormat format, int mip_level, int xoffset, int yoffset, uint32_t width, uint32_t height, const void* data ) {
	glTextureSubImage2D(
		id_, mip_level, xoffset, yoffset, static_cast<GLsizei>( width ),
		static_cast<GLsizei>( height ), TextureFormatToGLType( format ),
		GL_UNSIGNED_BYTE, data );
}

void Texture2D::GenerateMipmap() {
	glGenerateTextureMipmap( id_ );
}

}
