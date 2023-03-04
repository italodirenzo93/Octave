#ifndef OCTAVE_GRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICSCONTEXT_HPP

#include "Buffer.hpp"
// #include "Shader.hpp"

#include <array>
#include <string>

namespace Octave {

class GraphicsContext {
public:
	GraphicsContext() = default;
	virtual ~GraphicsContext() noexcept = default;

	virtual void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) const noexcept = 0;

	virtual void Draw( size_t vertex_count, size_t offset ) const noexcept = 0;
	virtual void DrawIndexed( size_t index_count, size_t offset, size_t base_vertex ) const noexcept = 0;

	[[nodiscard]] virtual std::string GetDescription() const noexcept = 0;
	[[nodiscard]] virtual std::array<int, 4> GetViewport() const noexcept = 0;

	virtual void SetDepthTestEnabled( bool enabled ) noexcept = 0;
	virtual void SetViewport( int x, int y, int width, int height ) noexcept = 0;

public:
	GraphicsContext( const GraphicsContext& ) = delete;
	GraphicsContext( GraphicsContext&& ) = delete;
	GraphicsContext& operator=( const GraphicsContext& ) = delete;
	GraphicsContext& operator=( GraphicsContext&& ) = delete;
};

}  // namespace Octave

#endif
