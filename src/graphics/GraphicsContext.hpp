#ifndef OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP

#include "Resources.hpp"

namespace Octave {

class GraphicsContext {
public:
	GraphicsContext() = default;
	virtual ~GraphicsContext() noexcept = default;

	GraphicsContext( const GraphicsContext& ) = delete;
	GraphicsContext& operator=( GraphicsContext&& ) = delete;

	virtual void Reset() noexcept = 0;
	virtual void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) noexcept = 0;

	virtual void Draw( size_t vertex_count, size_t offset ) noexcept = 0;
	virtual void DrawIndexed( size_t index_count, size_t offset,
					  size_t base_vertex ) noexcept = 0;

	[[nodiscard]] virtual std::array<int, 4> GetViewport() const noexcept = 0;

	virtual void SetProgram( const Program& program ) = 0;
	virtual void SetVertexBuffer( const Buffer& vbo ) = 0;
	virtual void SetIndexBuffer( const Buffer& ibo ) = 0;
	virtual void SetSampler( uint32_t unit, const Sampler& sampler ) = 0;
	virtual void SetTexture( uint32_t unit, const Texture2D& texture ) = 0;
	virtual void SetVertexLayout( const VertexLayout& layout ) = 0;
	virtual void SetViewport( int x, int y, int width, int height ) = 0;
};

}  // namespace Octave

#endif
