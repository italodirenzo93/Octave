#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "CommonInclude.hpp"

namespace graphics {
class Texture {
   public:
    explicit Texture( const char* fileName );
    Texture( const Texture& other );
    Texture( Texture&& other ) noexcept;
    ~Texture();

    uint32_t getID() const { return _id; }
    int32_t getFormat() const { return _format; }

    void activate( GLenum textureUnit ) const;

   private:
    uint32_t _id;
    int32_t _format;

    int32_t _width, _height;

   public:
    Texture& operator=( const Texture& other );
    Texture& operator=( Texture&& other ) noexcept;
};
}  // namespace graphics

#endif
