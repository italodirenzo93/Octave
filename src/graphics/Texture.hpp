#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "CommonInclude.hpp"

namespace octave::graphics {

class Texture {
	friend class Renderer;
	friend class Shader;

public:
	Texture() noexcept;
	Texture( Texture&& other ) noexcept;
	~Texture() noexcept;

	void LoadFromFile( const std::string& file_name );

private:
	uint32_t id_ = 0;
	int format_ = 0;

	int width_ = 0, height_ = 0;

public:
	Texture& operator=( Texture&& other ) noexcept;

	bool operator==( const Texture& other ) const noexcept;

private:
	NON_COPYABLE_CLASS( Texture )
};

}  // namespace octave::graphics

#endif
