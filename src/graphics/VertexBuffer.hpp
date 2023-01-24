#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace graphics {

struct VertexAttributeLayout {
};

class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer( const VertexBuffer& other );
    VertexBuffer( VertexBuffer&& other ) noexcept;
    ~VertexBuffer();

    uint32_t GetId() const { return id_; }
    uint32_t GetVertexCount() const { return vertex_count_; }

    template <class Vertex>
    void SetData( const VertexAttributeLayout* layout,
                  const std::vector<Vertex>& vertices ) {
        glBindBuffer( GL_ARRAY_BUFFER, id_ );

        // Set buffer data
        glBufferData( GL_ARRAY_BUFFER,
                      static_cast<int>( vertices.size() * sizeof( Vertex ) ),
                      reinterpret_cast<void*>( vertices.data() ),
                      GL_STATIC_DRAW );

        // Set up layout


        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

private:
    uint32_t id_ = 0;
    uint32_t vertex_count_ = 0;

public:
    VertexBuffer& operator=( const VertexBuffer& other );
    VertexBuffer& operator=( VertexBuffer&& other ) noexcept;
};

}  // namespace graphics

#endif
