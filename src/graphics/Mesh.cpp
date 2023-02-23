#include "Mesh.hpp"

namespace graphics {
using namespace std;

Mesh::Mesh( VertexBuffer&& vbo, IndexBuffer&& ibo,
            vector<Texture>&& texture ) noexcept {
    vbo_ = std::move( vbo );
    ibo_ = std::move( ibo );
    textures_ = std::move( texture );
}

Mesh::Mesh( const Mesh& other ) noexcept {
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
}

Mesh::Mesh( Mesh&& other ) noexcept {
    vbo_ = std::move( other.vbo_ );
    ibo_ = std::move( other.ibo_ );
    textures_ = std::move( other.textures_ );
}

void Mesh::Draw( const Shader& shader, Renderer& renderer ) const {
    assert( vbo_.GetVertexCount() > 0 );
    assert( ibo_.GetElementCount() > 0 );

    int texture_index = 0;
    for ( const auto& texture : textures_ ) {
        shader.SetTexture( "uTextures", texture_index++, texture );
    }

    renderer.DrawIndexedPrimitives( PrimitiveType::kTriangleList, vbo_, ibo_ );
}

Mesh& Mesh::operator=( const Mesh& other ) noexcept {
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;

    return *this;
}

Mesh& Mesh::operator=( Mesh&& other ) noexcept {
    vbo_ = std::move( other.vbo_ );
    ibo_ = std::move( other.ibo_ );
    textures_ = std::move( other.textures_ );

    return *this;
}
}  // namespace graphics
