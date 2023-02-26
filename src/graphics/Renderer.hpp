#ifndef OCTAVE_RENDERER_HPP
#define OCTAVE_RENDERER_HPP

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

namespace Octave {

class Renderer {
public:
	Renderer() = default;
	virtual ~Renderer() noexcept = default;

	virtual void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) const noexcept = 0;

	virtual void Draw( const Shader& shader, const VertexBuffer& vbo ) const noexcept = 0;
	virtual void DrawIndexed( const Shader& shader, const VertexBuffer& vbo,
					  const IndexBuffer& ibo ) const noexcept = 0;

	[[nodiscard]] virtual std::string GetDescription() const noexcept = 0;
	[[nodiscard]] virtual std::array<int, 4> GetViewport() const noexcept = 0;

	virtual void SetDepthTestEnabled( bool enabled ) noexcept = 0;
	virtual void SetViewport( int x, int y, int width, int height ) noexcept = 0;

public:
	Renderer( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer& operator=( Renderer&& ) = delete;
};

}  // namespace Octave

#endif
