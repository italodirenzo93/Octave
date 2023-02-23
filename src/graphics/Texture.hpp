#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "CommonInclude.hpp"

namespace graphics {
class Texture {
    friend class Renderer;

   public:
    Texture();
    Texture( Texture&& other ) noexcept;
    ~Texture();

    bool LoadFromFile( const std::filesystem::path& fileName );

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
