#include "VertexBuffer.hpp"

using namespace std;

namespace octave::graphics {
// NOTE: This must be kept in sync with the input layout of the vertex shader!
static const std::unordered_map<LayoutSemantic, uint32_t> kAttributeBindings{
	{ LayoutSemantic::kPosition, 0 },
	{ LayoutSemantic::kColor, 1 },
	{ LayoutSemantic::kTexCoord, 2 },
	{ LayoutSemantic::kNormal, 3 } };

static inline void CopyVertexBuffer( GLuint source, GLuint target ) {
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

VertexBuffer::VertexBuffer() noexcept {
	glGenBuffers( 1, &vbo_ );
	glGenVertexArrays( 1, &vao_ );
}

VertexBuffer::VertexBuffer( const VertexBuffer& other ) noexcept {
	glPushDebugGroup( GL_DEBUG_SOURCE_APPLICATION, 0, -1,
					  "Vertex buffer copy constructor" );

	glGenBuffers( 1, &vbo_ );
	glGenVertexArrays( 1, &vao_ );

	CopyVertexBuffer( other.vbo_, vbo_ );

	vertex_count_ = other.vertex_count_;
	layout_ = other.layout_;

	if ( !layout_.empty() ) {
		glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

		SetVertexAttributes( layout_, layout_.size() * sizeof( layout_[0] ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	glPopDebugGroup();
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
	glPushDebugGroup( GL_DEBUG_SOURCE_APPLICATION, 0, -1,
					  "SetVertexAttributes" );

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

	glPopDebugGroup();
}

VertexBuffer& VertexBuffer::operator=( const VertexBuffer& other ) noexcept {
	SELF_REFERENCE_CHECK( other );

	glPushDebugGroup( GL_DEBUG_SOURCE_APPLICATION, 0, -1,
					  "Vertex buffer copy assignment" );

	glGenBuffers( 1, &vbo_ );

	CopyVertexBuffer( other.vbo_, vbo_ );

	if ( !other.layout_.empty() ) {
		glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

		SetVertexAttributes( layout_, layout_.size() * sizeof( layout_[0] ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	vertex_count_ = other.vertex_count_;
	layout_ = other.layout_;

	glPopDebugGroup();

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
