#ifndef OCTAVE_TEXTURE_HPP
#define OCTAVE_TEXTURE_HPP

#include <string>

namespace Octave {

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

public:
	Texture( const Texture& ) = delete;
	Texture& operator=( const Texture& ) = delete;
};

}  // namespace Octave

#endif
