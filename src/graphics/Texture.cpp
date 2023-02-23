#include "Texture.hpp"

namespace graphics {
using namespace std;

Texture::Texture() {
    glGenTextures( 1, &id_ );
}

Texture::~Texture() {
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

bool Texture::LoadFromFile( const std::filesystem::path& fileName ) {
    int n_channels;
    stbi_uc* image =
        stbi_load( fileName.c_str(), &width_, &height_, &n_channels, STBI_default );

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

        stbi_image_free( image );
    } else {
        std::cout << "Unable to load texture at " << fileName << std::endl;
        stbi_image_free( image );

        return false;
    }

    return true;
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
}  // namespace graphics
