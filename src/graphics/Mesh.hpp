#ifndef MESH_HPP
#define MESH_HPP

#include "CommonInclude.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

namespace graphics {

class Mesh {
public:
    Mesh() = default;
    Mesh( VertexBuffer&& vbo, IndexBuffer&& ibo, Texture&& texture ) noexcept;
    Mesh( const Mesh& other ) noexcept;
    Mesh( Mesh&& other ) noexcept;
    ~Mesh() = default;

    void Draw( const Shader& shader, Renderer& renderer ) const;

private:
    VertexBuffer vbo_;
    IndexBuffer ibo_;
    Texture texture_;

public:
    Mesh& operator=( const Mesh& other ) noexcept;
    Mesh& operator=( Mesh&& other ) noexcept;
};

} // namespace graphics

#endif
