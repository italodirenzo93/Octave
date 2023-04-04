#include "pch/pch.hpp"
#include "Texture2D.hpp"

namespace Octave {

Texture2D::Texture2D( const TextureDescription2D& desc ) noexcept
	: handle_( 0 ), desc_( desc ) {
}

void Texture2D::SetData( TextureFormat format, int mip_level, int xoffset,
						 int yoffset, uint32_t width, uint32_t height,
						 const void* data ) {
	assert( glIsTexture( handle_ ) );

	glBindTexture( GL_TEXTURE_2D, handle_ );

	glTexSubImage2D( GL_TEXTURE_2D, mip_level, xoffset, yoffset,
					 static_cast<GLsizei>( width ),
					 static_cast<GLsizei>( height ),
					 format == TextureFormat::Rgb ? GL_RGB : GL_RGBA,
					 GL_UNSIGNED_BYTE, data );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

}  // namespace Octave
