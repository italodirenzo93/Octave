#ifndef OCTAVE_TEXTUREMANAGER_HPP
#define OCTAVE_TEXTUREMANAGER_HPP

#include "graphics/Texture.hpp"

namespace Octave {

class TextureManager {
public:
	static void LoadFromFile( Texture& texture, const std::string& path );
};

}

#endif
