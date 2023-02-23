#include "IndexBuffer.hpp"

using namespace std;

namespace Octave {

static inline void CopyIndexBuffer( GLuint source, GLuint target ) {
	GLsizei buffer_size;

	glBindBuffer( GL_COPY_READ_BUFFER, source );
	glGetBufferParameteriv( GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &buffer_size );

	glBindBuffer( GL_COPY_WRITE_BUFFER, target );
	glBufferData( GL_COPY_WRITE_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW );

	glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
						 buffer_size );

	glBindBuffer( GL_COPY_READ_BUFFER, 0 );
	glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );
}

IndexBuffer::IndexBuffer() noexcept {
	glGenBuffers( 1, &id_ );
}

IndexBuffer::IndexBuffer( const IndexBuffer& other ) noexcept {
	glGenBuffers( 1, &id_ );

	CopyIndexBuffer( other.id_, id_ );

	element_count_ = other.element_count_;
}

IndexBuffer::IndexBuffer( IndexBuffer&& other ) noexcept {
	id_ = other.id_;
	other.id_ = 0;

	element_count_ = 0;
	other.element_count_ = 0;
}

IndexBuffer::~IndexBuffer() noexcept {
	glDeleteBuffers( 1, &id_ );
}

std::vector<uint32_t> IndexBuffer::GetData() const noexcept {
	vector<uint32_t> data( element_count_ );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glGetBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0,
						static_cast<int>( element_count_ * sizeof( uint32_t ) ),
						data.data() );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	return data;
}

void IndexBuffer::SetData(
	std::initializer_list<uint32_t> initializerList ) noexcept {
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<int>( initializerList.size() * sizeof( uint32_t ) ),
		reinterpret_cast<const void*>( initializerList.begin() ),
		GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	element_count_ = static_cast<uint32_t>( initializerList.size() );
}

void IndexBuffer::SetData( const std::vector<uint32_t>& indices ) noexcept {
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				  static_cast<int>( indices.size() * sizeof( uint32_t ) ),
				  reinterpret_cast<const void*>( indices.data() ),
				  GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	element_count_ = static_cast<uint32_t>( indices.size() );
}

IndexBuffer& IndexBuffer::operator=( const IndexBuffer& other ) noexcept {
	SELF_REFERENCE_CHECK( other );

	glGenBuffers( 1, &id_ );

	CopyIndexBuffer( other.id_, id_ );

	element_count_ = other.element_count_;

	return *this;
}

IndexBuffer& IndexBuffer::operator=( IndexBuffer&& other ) noexcept {
	id_ = other.id_;
	other.id_ = 0;

	element_count_ = other.element_count_;
	other.element_count_ = 0;

	return *this;
}

bool IndexBuffer::operator==( const IndexBuffer& other ) const noexcept {
	return id_ == other.id_ && element_count_ && other.element_count_;
}

}  // namespace Octave
