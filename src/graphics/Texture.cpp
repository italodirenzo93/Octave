#include "Texture.hpp"

namespace graphics {
using namespace std;

Texture::Texture( const char* file_name )
    : id_( 0 ), format_( GL_RGBA ), width_( 0 ), height_( 0 ) {
    glGenTextures( 1, &id_ );

    int n_channels;
    stbi_uc* image =
        stbi_load( file_name, &width_, &height_, &n_channels, STBI_default );

    if (image) {
        if (n_channels == 1)
            format_ = GL_RED;
        else if (n_channels == 3)
            format_ = GL_RGB;
        else if (n_channels == 4)
            format_ = GL_RGBA;

        glBindTexture( GL_TEXTURE_2D, id_ );
        glTexImage2D( GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_,
                      GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, 0 );

        stbi_image_free( image );
    } else {
        std::cout << "Unable to load texture at " << file_name << std::endl;
        stbi_image_free( image );
    }
}

Texture::~Texture() {
    if (id_ != 0) {
        glDeleteTextures( 1, &id_ );
    }
}

Texture::Texture( const Texture& other )
    : id_( 0 ),
      format_( other.format_ ),
      width_( other.width_ ),
      height_( other.height_ ) {
    void* image = nullptr;
    glBindTexture( GL_TEXTURE_2D, other.id_ );
    glGetTexImage( GL_TEXTURE_2D, 0, other.format_, GL_UNSIGNED_BYTE, image );

    if (image) {
        glGenTextures( 1, &id_ );
        glBindTexture( GL_TEXTURE_2D, id_ );
        glTexImage2D( GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_,
                      GL_UNSIGNED_BYTE, image );
    } else {
        cout << "ERROR::TEXTURE: Could not copy texture with ID " << other.id_
            << endl;
    }

    glBindTexture( GL_TEXTURE_2D, 0 );
}

Texture::Texture( Texture&& other ) noexcept
    : id_( other.id_ ),
      format_( other.format_ ),
      width_( other.width_ ),
      height_( other.height_ ) {
    other.id_ = 0;
    other.format_ = GL_RGBA;
    other.width_ = 0;
    other.height_ = 0;
}

void Texture::activate( GLenum texture_unit ) const {
    glActiveTexture( texture_unit );
    glBindTexture( GL_TEXTURE_2D, id_ );
}

Texture& Texture::operator=( const Texture& other ) {
    void* image = nullptr;
    glBindTexture( GL_TEXTURE_2D, other.id_ );
    glGetTexImage( GL_TEXTURE_2D, 0, other.format_, GL_UNSIGNED_BYTE, image );

    if (image) {
        format_ = other.format_;
        width_ = other.width_;
        height_ = other.height_;

        glGenTextures( 1, &id_ );
        glBindTexture( GL_TEXTURE_2D, id_ );
        glTexImage2D( GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_,
                      GL_UNSIGNED_BYTE, image );
    } else {
        cout << "ERROR::TEXTURE: Could not copy texture with ID " << other.id_
            << endl;
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    return *this;
}

Texture& Texture::operator=( Texture&& other ) noexcept {
    id_ = other.id_;
    format_ = other.format_;
    width_ = other.width_;
    height_ = other.height_;

    other.id_ = 0;
    other.format_ = GL_RGBA;
    other.width_ = 0;
    other.height_ = 0;

    return *this;
}
} // namespace graphics
