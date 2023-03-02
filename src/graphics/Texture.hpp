#ifndef OCTAVE_TEXTURE_HPP
#define OCTAVE_TEXTURE_HPP

#include <string>

namespace Octave {

enum class TextureFormat { kRgb, kRgba, kBgra };

class Texture {
public:
	Texture() noexcept = default;
	Texture( const Texture& ) = delete;
	Texture( Texture&& other ) noexcept = default;
	virtual ~Texture() noexcept = default;

	Texture& operator=( const Texture& ) = delete;
	Texture& operator=( Texture&& other ) noexcept = default;

	[[nodiscard]] virtual TextureFormat GetFormat() const noexcept = 0;

	virtual void SetData( TextureFormat format, int width, int height, bool use_mipmaps, const void* pixels ) = 0;
};

}  // namespace Octave

#endif
