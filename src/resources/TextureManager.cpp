#include "pch/pch.hpp"
#include "TextureManager.hpp"

#include <stb_image.h>

namespace Octave {

void TextureManager::LoadFromFile( Texture& texture, const std::string& path ) {
	// Because Open GL coordinates are weird...
	stbi_set_flip_vertically_on_load( true );

	int n_channels, width, height;
	stbi_uc* image =
		stbi_load( path.c_str(), &width, &height, &n_channels, STBI_default );

	auto format = TextureFormat::kRgba;
	if ( n_channels == 3 ) {
		format = TextureFormat::kRgb;
	}

	if ( image ) {
		texture.SetData( format, width, height, true, image );
	} else {
		stbi_image_free( image );
		throw Exception( "Unable to load texture at " + path );
	}

	stbi_image_free( image );
}

}  // namespace Octave
