#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "CommonInclude.hpp"

namespace graphics {

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

private:
    NON_COPYABLE_CLASS( Texture )
};

}  // namespace graphics

#endif
