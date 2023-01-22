#include "Texture.hpp"

namespace graphics {
using namespace std;

Texture::Texture( const char* fileName )
    : _id( 0 ), _format( GL_RGBA ), _width( 0 ), _height( 0 ) {
    glGenTextures( 1, &_id );

    int nChannels;
    auto image =
        stbi_load( fileName, &_width, &_height, &nChannels, STBI_default );

    if (image) {
        if (nChannels == 1)
            _format = GL_RED;
        else if (nChannels == 3)
            _format = GL_RGB;
        else if (nChannels == 4)
            _format = GL_RGBA;

        glBindTexture( GL_TEXTURE_2D, _id );
        glTexImage2D( GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
                      GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, 0 );

        stbi_image_free( image );
    } else {
        std::cout << "Unable to load texture at " << fileName << std::endl;
        stbi_image_free( image );
    }
}

Texture::~Texture() {
    if (_id != 0) {
        glDeleteTextures( 1, &_id );
    }
}

Texture::Texture( const Texture& other )
    : _id( 0 ),
      _format( other._format ),
      _width( other._width ),
      _height( other._height ) {
    void* image = nullptr;
    glBindTexture( GL_TEXTURE_2D, other._id );
    glGetTexImage( GL_TEXTURE_2D, 0, other._format, GL_UNSIGNED_BYTE, image );

    if (image) {
        glGenTextures( 1, &_id );
        glBindTexture( GL_TEXTURE_2D, _id );
        glTexImage2D( GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
                      GL_UNSIGNED_BYTE, image );
    } else {
        cout << "ERROR::TEXTURE: Could not copy texture with ID " << other._id
            << endl;
    }

    glBindTexture( GL_TEXTURE_2D, 0 );
}

Texture::Texture( Texture&& other ) noexcept
    : _id( other._id ),
      _format( other._format ),
      _width( other._width ),
      _height( other._height ) {
    other._id = 0;
    other._format = GL_RGBA;
    other._width = 0;
    other._height = 0;
}

void Texture::activate( GLenum textureUnit ) const {
    glActiveTexture( textureUnit );
    glBindTexture( GL_TEXTURE_2D, _id );
}

Texture& Texture::operator=( const Texture& other ) {
    void* image = nullptr;
    glBindTexture( GL_TEXTURE_2D, other._id );
    glGetTexImage( GL_TEXTURE_2D, 0, other._format, GL_UNSIGNED_BYTE, image );

    if (image) {
        _format = other._format;
        _width = other._width;
        _height = other._height;

        glGenTextures( 1, &_id );
        glBindTexture( GL_TEXTURE_2D, _id );
        glTexImage2D( GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
                      GL_UNSIGNED_BYTE, image );
    } else {
        cout << "ERROR::TEXTURE: Could not copy texture with ID " << other._id
            << endl;
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    return *this;
}

Texture& Texture::operator=( Texture&& other ) noexcept {
    _id = other._id;
    _format = other._format;
    _width = other._width;
    _height = other._height;

    other._id = 0;
    other._format = GL_RGBA;
    other._width = 0;
    other._height = 0;

    return *this;
}
} // namespace graphics
