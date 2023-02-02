#include "VertexBuffer.hpp"

namespace octave::graphics {
// NOTE: This must be kept in sync with the input layout of the vertex shader!
static const std::map<LayoutSemantic, uint32_t> kAttributeBindings{
	{ LayoutSemantic::kPosition, 0 },
	{ LayoutSemantic::kColor, 1 },
	{ LayoutSemantic::kTexCoord, 2 },
	{ LayoutSemantic::kNormal, 3 } };

VertexBuffer::VertexBuffer() noexcept {
	glGenBuffers( 1, &vbo_ );
	glGenVertexArrays( 1, &vao_ );
}

VertexBuffer::VertexBuffer( const VertexBuffer& other ) noexcept {
	glGenBuffers( 1, &vbo_ );
	glGenVertexArrays( 1, &vao_ );

	glBindBuffer( GL_COPY_READ_BUFFER, other.vbo_ );
	glBindBuffer( GL_COPY_WRITE_BUFFER, vbo_ );

	glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
						 other.vertex_count_ );

	glBindBuffer( GL_COPY_READ_BUFFER, 0 );
	glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

	if ( !other.layout_.empty() ) {
		SetVertexAttributes( layout_, layout_.size() * sizeof( layout_[0] ) );
	}

	vertex_count_ = other.vertex_count_;
	layout_ = other.layout_;
}

VertexBuffer::VertexBuffer( VertexBuffer&& other ) noexcept {
	vbo_ = other.vbo_;
	other.vbo_ = 0;

	vao_ = other.vao_;
	other.vao_ = 0;

	vertex_count_ = other.vertex_count_;
	other.vertex_count_ = 0;

	layout_ = std::move( other.layout_ );
}

VertexBuffer::~VertexBuffer() noexcept {
	glDeleteBuffers( 1, &vbo_ );
	glDeleteVertexArrays( 1, &vao_ );
}

void VertexBuffer::SetVertexAttributes( const VertexLayout& layout,
										size_t stride ) {
	glBindVertexArray( vao_ );

	size_t offset_in_bytes = 0;

	for ( auto& binding : layout ) {
		auto iter = kAttributeBindings.find( binding.semantic );
		if ( iter == kAttributeBindings.end() ) continue;

		const uint32_t binding_point = iter->second;

		glVertexAttribPointer(
			binding_point, static_cast<int>( binding.size ), binding.type,
			binding.normalized, static_cast<int>( stride ),
			reinterpret_cast<const void*>( offset_in_bytes ) );
		glEnableVertexAttribArray( binding_point );

		offset_in_bytes +=
			binding.size * LayoutBinding::GetSizeOfType( binding.type );
	}

	layout_ = layout;

	glBindVertexArray( 0 );
}

VertexBuffer& VertexBuffer::operator=( const VertexBuffer& other ) noexcept {
	SELF_REFERENCE_CHECK( other );

	glGenBuffers( 1, &vbo_ );

	glBindBuffer( GL_COPY_READ_BUFFER, other.vbo_ );
	glBindBuffer( GL_COPY_WRITE_BUFFER, vbo_ );

	glCopyBufferSubData( GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
						 other.vertex_count_ );

	glBindBuffer( GL_COPY_READ_BUFFER, 0 );
	glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );

	if ( !other.layout_.empty() ) {
		SetVertexAttributes( layout_, layout_.size() * sizeof( layout_[0] ) );
	}

	vertex_count_ = other.vertex_count_;
	layout_ = other.layout_;

	return *this;
}

VertexBuffer& VertexBuffer::operator=( VertexBuffer&& other ) noexcept {
	vbo_ = other.vbo_;
	other.vbo_ = 0;

	vao_ = other.vao_;
	other.vao_ = 0;

	vertex_count_ = other.vertex_count_;
	other.vertex_count_ = 0;

	layout_ = std::move( other.layout_ );

	return *this;
}

bool VertexBuffer::operator==( const VertexBuffer& other ) const noexcept {
	return vbo_ == other.vbo_ && vao_ == other.vao_ &&
		   vertex_count_ == other.vertex_count_ && layout_ == other.layout_;
}

}  // namespace octave::graphics
