#include "Mesh.hpp"

namespace graphics {
using namespace std;

Mesh::Mesh( const std::vector<Vertex>& vertex_data,
            const std::vector<uint32_t>& index_data,
            const std::vector<Texture>& textures ) {
    vertices_ = vertex_data;
    indices_ = index_data;
    this->textures_ = textures;

    SetupMesh();
}

Mesh::Mesh( const Mesh& other ) {
    vertices_ = other.vertices_;
    indices_ = other.indices_;
    textures_ = other.textures_;

    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;

    SetupMesh();
}

Mesh::Mesh( Mesh&& other ) noexcept {
    vertices_ = std::move( other.vertices_ );
    indices_ = std::move( other.indices_ );
    textures_ = std::move( other.textures_ );

    vao_ = std::move( other.vao_ );
    vbo_ = std::move( other.vbo_ );
    ibo_ = std::move( other.ibo_ );
}

void Mesh::draw( const Shader& program ) const {
    uint32_t diffuse_nr = 1;
    uint32_t specular_nr = 1;
    for (size_t i = 0; i < textures_.size(); i++) {
        glActiveTexture( GL_TEXTURE0 + i ); // activate the proper texture unit
        string name = "uMaterial.";
        name += textures_[i].type;

        if (textures_[i].type == "texture_diffuse")
            name += to_string( diffuse_nr++ );
        else if (textures_[i].type == "texture_specular")
            name += to_string( specular_nr++ );

        program.SetInt( name, i );
        glBindTexture( GL_TEXTURE_2D, textures_[i].id );
    }
    glActiveTexture( GL_TEXTURE0 );

    glBindVertexArray( vao_.id_ );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_.id_ );

    glDrawElements( GL_TRIANGLES, static_cast<int>(indices_.size()),
                    GL_UNSIGNED_INT, nullptr );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_.id_ );
    glBindVertexArray( 0 );
}

void Mesh::SetupMesh() {
    vao_.AddBinding( {POSITION, 3, GL_FLOAT, false} )
        .AddBinding( {TEXCOORD, 2, GL_FLOAT, false} )
        .AddBinding( {NORMAL, 3, GL_FLOAT, false} );

    vbo_.SetData( vao_, vertices_ );
    ibo_.SetData( indices_ );
}

Mesh& Mesh::operator=( const Mesh& other ) {
    vertices_ = other.vertices_;
    indices_ = other.indices_;
    textures_ = other.textures_;

    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ibo_ = other.ibo_;

    SetupMesh();

    return *this;
}

Mesh& Mesh::operator=( Mesh&& other ) noexcept {
    vertices_ = std::move( other.vertices_ );
    indices_ = std::move( other.indices_ );
    textures_ = std::move( other.textures_ );

    vao_ = std::move( other.vao_ );
    vbo_ = std::move( other.vbo_ );
    ibo_ = std::move( other.ibo_ );

    return *this;
}
} // namespace graphics
