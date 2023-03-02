#ifndef OCTAVE_TEXTUREGL_HPP
#define OCTAVE_TEXTUREGL_HPP

#include "graphics/Texture.hpp"

namespace Octave::Impl {

class TextureGL : public Texture {
public:
	TextureGL() noexcept;
	~TextureGL() noexcept override;

	[[nodiscard]] uint32_t GetId() const noexcept { return id_; }
	[[nodiscard]] TextureFormat GetFormat() const noexcept override;

	void SetData( TextureFormat format, int width, int height, bool use_mipmaps, const void* pixels ) override;

private:
	uint32_t id_ = 0;
};

}

#endif
