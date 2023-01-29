#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include "CommonInclude.hpp"

namespace octave::graphics {

enum class LayoutSemantic { kPosition, kColor, kTexCoord, kNormal };

struct LayoutBinding {
    LayoutSemantic semantic;
    unsigned int size;
    unsigned int type;
    bool normalized;

    bool operator==( const LayoutBinding& other ) const {
        return semantic == other.semantic && size == other.size &&
               type == other.type && normalized == other.normalized;
    }

    static uint32_t GetSizeOfType( uint32_t type ) {
        switch ( type ) {
            case GL_FLOAT:
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            default:
                assert( false );
                return 0;
        }
    }
};

class VertexBuffer {
    friend class Renderer;
    friend class Mesh;

public:
    VertexBuffer() noexcept;
    VertexBuffer( const VertexBuffer& other ) noexcept;
    VertexBuffer( VertexBuffer&& other ) noexcept;
    ~VertexBuffer() noexcept;

	using VertexLayout = std::vector<LayoutBinding>;

    [[nodiscard]] uint32_t GetVertexCount() const noexcept { return vertex_count_; }

    template <typename Vertex>
    std::vector<Vertex> GetData() const noexcept;

    template <typename Vertex>
    void SetData( const VertexLayout& layout,
                  std::initializer_list<Vertex> vertices ) noexcept;
    template <typename Vertex>
    void SetData( const VertexLayout& layout,
                  const std::vector<Vertex>& vertices ) noexcept;

private:
    uint32_t vbo_ = 0;
    uint32_t vao_ = 0;
    uint32_t vertex_count_ = 0;
    VertexLayout layout_;

public:
    VertexBuffer& operator=( const VertexBuffer& other ) noexcept;
    VertexBuffer& operator=( VertexBuffer&& other ) noexcept;

    bool operator==( const VertexBuffer& other ) const noexcept;

private:
    void SetVertexAttributes( const VertexLayout& layout,
                                     size_t stride );
};

template <typename Vertex>
std::vector<Vertex> VertexBuffer::GetData() const noexcept {
    std::vector<Vertex> data( vertex_count_ );

    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    glGetBufferSubData( GL_ARRAY_BUFFER, 0,
                        static_cast<int>(vertex_count_ * sizeof( Vertex )),
                        data.data() );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return data;
}

template <typename Vertex>
void VertexBuffer::SetData( const VertexLayout& layout,
                            std::initializer_list<Vertex> vertices ) noexcept {
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    // Set buffer data
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof( Vertex )),
        reinterpret_cast<const void*>(vertices.begin()), GL_STATIC_DRAW );

    // Set up layout
    SetVertexAttributes( layout, sizeof( Vertex ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    vertex_count_ = static_cast<uint32_t>(vertices.size());
}

template <typename Vertex>
void VertexBuffer::SetData( const VertexLayout& layout,
                            const std::vector<Vertex>& vertices ) noexcept {
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    // Set buffer data
    glBufferData(
        GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof( Vertex )),
        reinterpret_cast<const void*>(vertices.data()), GL_STATIC_DRAW );

    // Set up layout
    SetVertexAttributes( layout, sizeof( Vertex ) );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    vertex_count_ = static_cast<uint32_t>(vertices.size());
}

} // namespace octave::graphics

#endif
