#ifndef OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP

#include "Resources.hpp"
#include "core/Types.hpp"

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

	void SetPipeline( SharedRef<Pipeline> pipeline );
	void SetVertexArray( SharedRef<VertexArray> vao );
	void SetSampler( uint32_t unit, SharedRef<Sampler> sampler );
	void SetTexture( uint32_t unit, SharedRef<Texture2D> texture );
	void SetViewport( int x, int y, int width, int height ) noexcept;

	void Signal( SharedRef<Fence> fence );
	void Wait( SharedRef<Fence> fence, uint64_t timeout );

private:
	GraphicsContext();
};

}  // namespace Octave

#endif
