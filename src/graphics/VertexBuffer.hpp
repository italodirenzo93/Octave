#ifndef OCTAVE_VERTEXBUFFER_HPP
#define OCTAVE_VERTEXBUFFER_HPP

#include <cstdint>
#include <vector>

namespace Octave {

enum class VertexAttributeName { kPosition, kColor, kTexCoord, kNormal };
enum class VertexAttributeType { kFloat, kUint, kUbyte };

struct LayoutBinding {
	VertexAttributeName name;
	uint32_t size;
	VertexAttributeType type;
	bool normalized;

	bool operator==( const LayoutBinding& other ) const noexcept;
};

class VertexBuffer {
	friend class Mesh;

public:
	VertexBuffer() noexcept = default;
	VertexBuffer( const VertexBuffer& other ) = default;
	VertexBuffer( VertexBuffer&& other ) noexcept = default;
	virtual ~VertexBuffer() noexcept = default;

	VertexBuffer& operator=( const VertexBuffer& other ) = default;
	VertexBuffer& operator=( VertexBuffer&& other ) noexcept = default;

	using VertexLayout = std::vector<LayoutBinding>;

	[[nodiscard]] virtual uint32_t GetVertexCount() const noexcept = 0;

	template <typename Vertex>
	std::vector<Vertex> GetData() const noexcept;

	template <typename Vertex>
	void SetData( const VertexLayout& layout,
				  const std::vector<Vertex>& vertices ) noexcept;

protected:
	virtual void ReadBufferData( uint32_t vertex_count, size_t vertex_size,
								 void* data ) = 0;
	virtual void SetBufferData( const VertexLayout& layout,
								uint32_t vertex_count, size_t vertex_size,
								const void* data ) = 0;
};

template <typename Vertex>
std::vector<Vertex> VertexBuffer::GetData() const noexcept {
	const uint32_t vertex_count = GetVertexCount();

	std::vector<Vertex> data( vertex_count );

	ReadBufferData( vertex_count, sizeof( Vertex ), data.data() );

	return data;
}

template <typename Vertex>
void VertexBuffer::SetData( const VertexLayout& layout,
							const std::vector<Vertex>& vertices ) noexcept {
	SetBufferData( layout, vertices.size(), sizeof( Vertex ), vertices.data() );
}

}  // namespace Octave

#endif
