#include "Mesh.hpp"

namespace graphics {
using namespace std;

Mesh::Mesh( const std::vector<Vertex>& vertexData,
            const std::vector<uint32_t>& indexData,
            const std::vector<Texture>& textures ) {
    vertices = vertexData;
    indices = indexData;
    this->textures = textures;

    setupMesh();
}

Mesh::Mesh( const Mesh& other ) {
    vertices = other.vertices;
    indices = other.indices;
    textures = other.textures;

    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;

    setupMesh();
}

Mesh::Mesh( Mesh&& other ) noexcept {
    vertices = std::move( other.vertices );
    indices = std::move( other.indices );
    textures = std::move( other.textures );

    m_vao = std::move( other.m_vao );
    m_vbo = std::move( other.m_vbo );
    m_ibo = std::move( other.m_ibo );
}

void Mesh::draw( const Shader& program ) const {
    uint32_t diffuseNr = 1;
    uint32_t specularNr = 1;
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture( GL_TEXTURE0 + i ); // activate the proper texture unit
        string name = "uMaterial.";
        name += textures[i].type;

        if (textures[i].type == "texture_diffuse")
            name += to_string( diffuseNr++ );
        else if (textures[i].type == "texture_specular")
            name += to_string( specularNr++ );

        program.setInt( name, i );
        glBindTexture( GL_TEXTURE_2D, textures[i].id );
    }
    glActiveTexture( GL_TEXTURE0 );

    m_vao.bind();
    m_ibo.bind();

    glDrawElements( GL_TRIANGLES, static_cast<uint32_t>(indices.size()),
                    GL_UNSIGNED_INT, nullptr );

    m_ibo.unbind();
    m_vao.unbind();
}

void Mesh::setupMesh() {
    m_vbo.setData( vertices );
    m_ibo.setData( indices );

    m_vao.addBinding( {POSITION, 3, GL_FLOAT, false} )
         .addBinding( {TEXCOORD, 2, GL_FLOAT, false} )
         .addBinding( {NORMAL, 3, GL_FLOAT, false} );

    m_vao.mapToBuffer( m_vbo );
}

Mesh& Mesh::operator=( const Mesh& other ) {
    vertices = other.vertices;
    indices = other.indices;
    textures = other.textures;

    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;

    setupMesh();

    return *this;
}

Mesh& Mesh::operator=( Mesh&& other ) noexcept {
    vertices = std::move( other.vertices );
    indices = std::move( other.indices );
    textures = std::move( other.textures );

    m_vao = std::move( other.m_vao );
    m_vbo = std::move( other.m_vbo );
    m_ibo = std::move( other.m_ibo );

    return *this;
}
} // namespace graphics
