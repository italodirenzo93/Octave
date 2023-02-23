#include "Mesh.hpp"

namespace graphics {
using namespace std;

Mesh::Mesh( VertexBuffer&& vbo, IndexBuffer&& ibo, Texture&& texture ) noexcept {
    vbo_ = std::move( vbo );
    ibo_ = std::move( ibo );
    texture_ = std::move( texture );
}

Mesh::Mesh( const Mesh& other ) noexcept {
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;
}

Mesh::Mesh( Mesh&& other ) noexcept {
    vbo_ = std::move( other.vbo_ );
    ibo_ = std::move( other.ibo_ );
    texture_ = std::move( other.texture_ );
}

void Mesh::Draw( const Shader& shader, Renderer& renderer ) const {
    assert( vbo_.GetVertexCount() > 0 );
    assert( ibo_.GetElementCount() > 0 );

    shader.SetTexture( "uTextures", 0, texture_ );
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
    texture_ = std::move( other.texture_ );

    return *this;
}
}  // namespace graphics
