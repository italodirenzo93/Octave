#include "IndexBuffer.hpp"

using namespace std;

namespace graphics {
IndexBuffer::IndexBuffer() {
    glGenBuffers( 1, &id_ );
}

IndexBuffer::IndexBuffer( const IndexBuffer& other ) {
    glGenBuffers( 1, &id_ );

    glBindBuffer( GL_COPY_READ_BUFFER, other.id_ );
    glBindBuffer( GL_COPY_WRITE_BUFFER, id_ );

    glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                         other.element_count_ );

    glBindBuffer( GL_COPY_READ_BUFFER, 0 );
    glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

    element_count_ = other.element_count_;
}

IndexBuffer::IndexBuffer( IndexBuffer&& other ) noexcept {
    id_ = other.id_;
    other.id_ = 0;

    element_count_ = 0;
    other.element_count_ = 0;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers( 1, &id_ );
}

std::vector<uint32_t> IndexBuffer::GetData() const {
    vector<uint32_t> data( element_count_ );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glGetBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0,
                        static_cast<int>( element_count_ * sizeof( uint32_t ) ),
                        data.data() );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	return data;
}

void IndexBuffer::SetData(
    std::initializer_list<uint32_t> initializerList ) {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<int>( initializerList.size() * sizeof( uint32_t ) ),
        reinterpret_cast<const void*>( initializerList.begin() ),
        GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	element_count_ = static_cast<uint32_t>( initializerList.size() );
}

void IndexBuffer::SetData( const std::vector<uint32_t>& indices ) {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  static_cast<int>( indices.size() * sizeof( uint32_t ) ),
                  reinterpret_cast<const void*>( indices.data() ),
                  GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	element_count_ = static_cast<uint32_t>( indices.size() );
}

IndexBuffer& IndexBuffer::operator=( const IndexBuffer& other ) {
    glGenBuffers( 1, &id_ );

    glBindBuffer( GL_COPY_READ_BUFFER, other.id_ );
    glBindBuffer( GL_COPY_WRITE_BUFFER, id_ );

    glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                         other.element_count_ );

    glBindBuffer( GL_COPY_READ_BUFFER, 0 );
    glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

    element_count_ = other.element_count_;

    return *this;
}

IndexBuffer& IndexBuffer::operator=( IndexBuffer&& other ) noexcept {
    id_ = other.id_;
    other.id_ = 0;

    element_count_ = 0;
    other.element_count_ = 0;

    return *this;
}

}  // namespace graphics
