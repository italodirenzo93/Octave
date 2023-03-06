#ifndef OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICS_GRAPHICSCONTEXT_HPP

#include "core/Types.hpp"

#include "Buffer.hpp"
#include "Shader.hpp"
#include "VertexArrayLayout.hpp"

namespace Octave {

class GraphicsContext {
public:
	GraphicsContext() = default;
	virtual ~GraphicsContext() noexcept = default;

	virtual void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) const noexcept = 0;

	virtual void Draw( size_t vertex_count, size_t offset ) const noexcept = 0;
	virtual void DrawIndexed( size_t index_count, size_t offset, size_t base_vertex ) const noexcept = 0;

	[[nodiscard]] virtual std::array<int, 4> GetViewport() const noexcept = 0;

	virtual void SetDepthTestEnabled( bool enabled ) noexcept = 0;
	virtual void SetVertexBuffer( SharedRef<Buffer> vertex_buffer, size_t stride, SharedRef<VertexArrayLayout> layout ) = 0;
	virtual void SetIndexBuffer( SharedRef<Buffer> index_buffer ) = 0;
	virtual void SetVertexShader( SharedRef<Shader> vertex_shader ) = 0;
	virtual void SetFragmentShader( SharedRef<Shader> fragment_shader ) = 0;
	virtual void SetViewport( int x, int y, int width, int height ) noexcept = 0;

public:
	GraphicsContext( const GraphicsContext& ) = delete;
	GraphicsContext( GraphicsContext&& ) = delete;
	GraphicsContext& operator=( const GraphicsContext& ) = delete;
	GraphicsContext& operator=( GraphicsContext&& ) = delete;
};

}  // namespace Octave

#endif
