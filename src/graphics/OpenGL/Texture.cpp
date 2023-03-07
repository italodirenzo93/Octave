#include "pch/pch.hpp"
#include "Texture.hpp"

namespace Octave {

Texture::Texture() {
	glGenTextures( 1, &id_ );

}

Texture::~Texture() noexcept {
	glDeleteTextures( 1, &id_ );
}

}
