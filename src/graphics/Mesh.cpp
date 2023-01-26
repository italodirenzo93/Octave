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

void Mesh::Draw( Renderer& renderer ) const {
    //    uint32_t diffuse_nr = 1;
    //    uint32_t specular_nr = 1;
    //    for (size_t i = 0; i < textures_.size(); i++) {
    //        glActiveTexture( GL_TEXTURE0 + static_cast<int>(i) );
    //        // activate the proper texture unit
    //        string name = "uMaterial.";
    //        name += textures_[i].type;
    //
    //        if (textures_[i].type == "texture_diffuse")
    //            name += to_string( diffuse_nr++ );
    //        else if (textures_[i].type == "texture_specular")
    //            name += to_string( specular_nr++ );
    //
    //        program.SetInt( name, static_cast<int>(i) );
    //        glBindTexture( GL_TEXTURE_2D, textures_[i].id );
    //    }
    //    glActiveTexture( GL_TEXTURE0 );
    //
    //    glBindVertexArray( vbo_.vao_ );
    //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_.id_ );
    //
    //    glDrawElements( GL_TRIANGLES,
    //    static_cast<int>(ibo_.GetElementCount()),
    //                    GL_UNSIGNED_INT, nullptr );
    //
    //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_.id_ );
    //    glBindVertexArray( 0 );

    assert( vbo_.GetVertexCount() > 0 );
    assert( ibo_.GetElementCount() > 0 );

    renderer.SetTexture( texture_ );
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
