#ifndef MESH_HPP
#define MESH_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "VertexArrayLayout.hpp"

namespace graphics {
class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoords;
        glm::vec3 normal;
    };

    struct Texture {
        uint32_t id;
        std::string type;
        std::string path;
    };

public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    explicit Mesh( const std::vector<Vertex>& vertexData,
                   const std::vector<uint32_t>& indexData,
                   const std::vector<Texture>& textures );
    Mesh( const Mesh& other );
    Mesh( Mesh&& other ) noexcept;

    void draw( const Shader& program ) const;

private:
    VertexArrayLayout m_vao;
    VertexBuffer<Vertex> m_vbo;
    IndexBuffer m_ibo;

    void setupMesh();

public:
    Mesh& operator=( const Mesh& other );
    Mesh& operator=( Mesh&& other ) noexcept;
};
} // namespace graphics
#endif
