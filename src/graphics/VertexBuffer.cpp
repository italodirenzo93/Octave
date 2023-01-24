#include "VertexBuffer.hpp"

namespace graphics {

VertexBuffer::VertexBuffer() {
    glGenBuffers( 1, &id_ );
}

VertexBuffer::VertexBuffer( const VertexBuffer& other ) {
    glGenBuffers( 1, &id_ );

    glBindBuffer( GL_COPY_READ_BUFFER, other.id_ );
    glBindBuffer( GL_COPY_WRITE_BUFFER, id_ );

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, other.vertex_count_);

    glBindBuffer( GL_COPY_READ_BUFFER, 0 );
    glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

    vertex_count_ = other.vertex_count_;
}

VertexBuffer::VertexBuffer( VertexBuffer&& other ) noexcept {
    id_ = other.id_;
    other.id_ = 0;

    vertex_count_ = other.vertex_count_;
    other.vertex_count_ = 0;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers( 1, &id_ );
}

void VertexBuffer::SetVertexAttributes( const VertexArrayLayout& layout,
                                        size_t stride ) {
    glBindVertexArray( layout.id_ );

    size_t offset_in_bytes = 0;

    for ( auto& binding : layout.layout_bindings_ ) {
        auto iter = VertexArrayLayout::bindings_.find( binding.semantic );
        if ( iter == VertexArrayLayout::bindings_.end() ) continue;

        const uint32_t binding_point = iter->second;

        glVertexAttribPointer(
            binding_point, static_cast<int>( binding.size ), binding.type,
            binding.normalized, static_cast<int>( stride ),
            reinterpret_cast<const void*>( offset_in_bytes ) );
        glEnableVertexAttribArray( binding_point );

        offset_in_bytes +=
            binding.size * LayoutBinding::GetSizeOfType( binding.type );
    }

    glBindVertexArray( 0 );
}

VertexBuffer& VertexBuffer::operator=( const VertexBuffer& other ) {
    glGenBuffers( 1, &id_ );

    glBindBuffer( GL_COPY_READ_BUFFER, other.id_ );
    glBindBuffer( GL_COPY_WRITE_BUFFER, id_ );

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, other.vertex_count_);

    glBindBuffer( GL_COPY_READ_BUFFER, 0 );
    glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

    vertex_count_ = other.vertex_count_;

    return *this;
}

VertexBuffer& VertexBuffer::operator=( VertexBuffer&& other ) noexcept {
    id_ = other.id_;
    other.id_ = 0;

    vertex_count_ = other.vertex_count_;
    other.vertex_count_ = 0;

    return *this;
}

}  // namespace graphics
