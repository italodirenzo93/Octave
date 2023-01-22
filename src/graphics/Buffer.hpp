#ifndef BUFFER_H
#define BUFFER_H

#include "CommonInclude.hpp"

namespace graphics {
template <typename T, uint32_t BufferType>
class Buffer {
public:
    Buffer() {
        glGenBuffers( 1, &id_ );
    }

    Buffer( const Buffer& other ) { copy( other ); }

    Buffer( Buffer&& other ) noexcept
        : id_( other.id_ ) { other.id_ = 0; }

    ~Buffer() {
        glDeleteBuffers( 1, &id_ );
    }

    uint32_t GetId() const { return id_; }

    void SetData( const std::vector<T>& data ) {
        glBindBuffer( BufferType, id_ );
        glBufferData(
            BufferType, static_cast<uint32_t>(data.size() * sizeof( T )),
            reinterpret_cast<const void*>(data.data()), GL_STATIC_DRAW );
        glBindBuffer( BufferType, 0 );
    }

    void SetData( std::initializer_list<T> initializer ) {
        glBindBuffer( BufferType, id_ );
        glBufferData( BufferType,
                      static_cast<uint32_t>(initializer.size() * sizeof( T )),
                      initializer.begin(), GL_STATIC_DRAW );
        glBindBuffer( BufferType, 0 );
    }

    void Bind() const {
        glBindBuffer( BufferType, id_ );
    }

    void Unbind() const {
        glBindBuffer( BufferType, 0 );
    }

private:
    uint32_t id_ = 0;

    void copy( const Buffer& other ) {
        // Create a new buffer
        glGenBuffers( 1, &id_ );

        // Load the other buffer for reading
        int size = 0;
        glBindBuffer( GL_COPY_READ_BUFFER, other.id_ );
        glGetBufferParameteriv( GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size );

        // Load the new buffer for writing
        glBindBuffer( GL_COPY_WRITE_BUFFER, id_ );
        glBufferData( GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW );

        // Copy all data
        glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                             size );

        // Unbind buffers
        glBindBuffer( GL_COPY_READ_BUFFER, 0 );
        glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );
    }

public:
    Buffer& operator=( const Buffer& other ) {
        copy( other );
        return *this;
    }

    Buffer& operator=( Buffer&& other ) noexcept {
        id_ = other.id_;
        other.id_ = 0;

        return *this;
    }
};

template <typename Vertex>
class VertexBuffer : public Buffer<Vertex, GL_ARRAY_BUFFER> {
};

class IndexBuffer : public Buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> {
};
} // namespace graphics

#endif
