#ifndef OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP

#include "Resources.hpp"

namespace Octave {

class GraphicsContext {
	friend class GraphicsDevice;

public:
	~GraphicsContext() noexcept;

	GraphicsContext( const GraphicsContext& ) = delete;
	GraphicsContext& operator=( GraphicsContext&& ) = delete;

	void Reset() noexcept;
	void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) const noexcept;

	void Draw( size_t vertex_count, size_t offset ) const noexcept;
	void DrawIndexed( size_t index_count, size_t offset,
					  size_t base_vertex ) const noexcept;

	[[nodiscard]] std::array<int, 4> GetViewport() const noexcept;

	void SetProgram( const Program& program );
	void SetVertexBuffer( const VertexArray& vao, const Buffer& vbo );
	void SetIndexBuffer( const Buffer& ibo );
	void SetSampler( uint32_t unit, const Sampler& sampler );
	void SetTexture( uint32_t unit, const Texture2D& texture );
	void SetViewport( int x, int y, int width, int height ) noexcept;

private:
	GraphicsContext();
};

}  // namespace Octave

#endif
