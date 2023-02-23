#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "CommonInclude.hpp"

namespace graphics {
class Texture {
   public:
    explicit Texture( const char* file_name );
    Texture( const Texture& other );
    Texture( Texture&& other ) noexcept;
    ~Texture();

    uint32_t GetId() const { return id_; }
    int GetFormat() const { return format_; }

    void activate( GLenum texture_unit ) const;

   private:
    uint32_t id_;
    int32_t format_;

    int32_t width_, height_;

   public:
    Texture& operator=( const Texture& other );
    Texture& operator=( Texture&& other ) noexcept;
};
}  // namespace graphics

#endif
