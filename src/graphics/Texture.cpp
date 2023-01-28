#include "Texture.hpp"

#include "Exception.hpp"

namespace graphics {
using namespace std;

Texture::Texture() noexcept {
    glGenTextures( 1, &id_ );
}

Texture::~Texture() noexcept {
    glDeleteTextures( 1, &id_ );
}

Texture::Texture( Texture&& other ) noexcept
    : id_( other.id_ ),
      format_( other.format_ ),
      width_( other.width_ ),
      height_( other.height_ ) {
    other.id_ = 0;
    other.format_ = 0;
    other.width_ = 0;
    other.height_ = 0;
}

void Texture::LoadFromFile( const std::string& file_name ) {
    int n_channels;
    stbi_uc* image = stbi_load( file_name.c_str(), &width_, &height_,
                                &n_channels, STBI_default );

    if ( image ) {
        if ( n_channels == 1 )
            format_ = GL_RED;
        else if ( n_channels == 3 )
            format_ = GL_RGB;
        else if ( n_channels == 4 )
            format_ = GL_RGBA;

        glBindTexture( GL_TEXTURE_2D, id_ );
        glTexImage2D( GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_,
                      GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, 0 );
    } else {
        stbi_image_free( image );
        throw Exception( "Unable to load texture at " + file_name );
    }

    stbi_image_free( image );
}

Texture& Texture::operator=( Texture&& other ) noexcept {
    id_ = other.id_;
    format_ = other.format_;
    width_ = other.width_;
    height_ = other.height_;

    other.id_ = 0;
    other.format_ = 0;
    other.width_ = 0;
    other.height_ = 0;

    return *this;
}

bool Texture::operator==( const Texture& other ) const noexcept {
    return id_ == other.id_ && format_ == other.format_ &&
           width_ == other.width_ && height_ == other.height_;
}

}  // namespace graphics
