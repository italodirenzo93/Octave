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
