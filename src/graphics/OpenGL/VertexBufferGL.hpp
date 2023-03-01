#ifndef OCTAVE_VERTEXBUFFERGL_HPP
#define OCTAVE_VERTEXBUFFERGL_HPP

#include "graphics/VertexBuffer.hpp"

namespace Octave::Impl {

class VertexBufferGL : public VertexBuffer {
	friend class RendererGL;

public:
	VertexBufferGL() noexcept;
	~VertexBufferGL() noexcept override;

	[[nodiscard]] uint32_t GetVertexCount() const noexcept override;

protected:
	void ReadBufferData( uint32_t vertex_count, size_t vertex_size, void* data ) override;
	void SetBufferData( const VertexLayout& layout, uint32_t vertex_count, size_t vertex_size, const void* data ) override;

	void BindVertexArray( const VertexLayout& layout, size_t stride );

private:
	uint32_t vbo_ = 0;
	uint32_t vao_ = 0;
	uint32_t vertex_count_ = 0;
	VertexLayout layout_;
};

}

#endif
