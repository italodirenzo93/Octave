#ifndef MESH_HPP
#define MESH_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace graphics {
class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normal;
    };

    struct Texture {
        uint32_t id;
        std::string type;
        std::string path;
    };

public:
    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    std::vector<Texture> textures_;

    explicit Mesh( const std::vector<Vertex>& vertex_data,
                   const std::vector<uint32_t>& index_data,
                   const std::vector<Texture>& textures );
    Mesh( const Mesh& other );
    Mesh( Mesh&& other ) noexcept;
    ~Mesh() = default;

    void draw( const Shader& program ) const;

private:
    VertexBuffer vbo_;
    IndexBuffer ibo_;

    void SetupMesh();

public:
    Mesh& operator=( const Mesh& other );
    Mesh& operator=( Mesh&& other ) noexcept;
};
} // namespace graphics
#endif
