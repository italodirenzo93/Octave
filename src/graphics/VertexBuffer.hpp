#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include "CommonInclude.hpp"
#include "VertexArrayLayout.hpp"

namespace graphics {

class VertexBuffer {
    friend class Mesh;

public:
    VertexBuffer();
    VertexBuffer( const VertexBuffer& other );
    VertexBuffer( VertexBuffer&& other ) noexcept;
    ~VertexBuffer();

    uint32_t GetVertexCount() const { return vertex_count_; }

    template <typename Vertex>
    void SetData( const VertexArrayLayout& layout, std::initializer_list<Vertex> vertices );
    template <typename Vertex>
    void SetData( const VertexArrayLayout& layout, const std::vector<Vertex>& vertices );

private:
    uint32_t id_ = 0;
    uint32_t vertex_count_ = 0;

public:
    VertexBuffer& operator=( const VertexBuffer& other );
    VertexBuffer& operator=( VertexBuffer&& other ) noexcept;

private:
    static void SetVertexAttributes( const VertexArrayLayout& layout, size_t stride );
};

template <typename Vertex>
void VertexBuffer::SetData( const VertexArrayLayout& layout,
                            std::initializer_list<Vertex> vertices ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_ );

    // Set buffer data
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<int>( vertices.size() * sizeof( Vertex ) ),
        reinterpret_cast<const void*>( vertices.begin() ), GL_STATIC_DRAW );

    // Set up layout
    SetVertexAttributes( layout, sizeof( Vertex ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

	vertex_count_ = static_cast<uint32_t>( vertices.size() );
}

template <typename Vertex>
void VertexBuffer::SetData( const VertexArrayLayout& layout,
                            const std::vector<Vertex>& vertices ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_ );

    // Set buffer data
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<int>( vertices.size() * sizeof( Vertex ) ),
        reinterpret_cast<const void*>( vertices.data() ), GL_STATIC_DRAW );

    // Set up layout
    SetVertexAttributes( layout, sizeof( Vertex ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

	vertex_count_ = static_cast<uint32_t>( vertices.size() );
}

}  // namespace graphics

#endif
