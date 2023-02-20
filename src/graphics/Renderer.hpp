#ifndef OCTAVE_RENDERER_HPP
#define OCTAVE_RENDERER_HPP

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBuffer.hpp"

namespace Octave {

class Renderer {
public:
	Renderer() noexcept;
	~Renderer() noexcept = default;

	void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f, float a = 1.0f ) const noexcept;

	void Draw( const VertexBuffer& vbo ) const noexcept;
	void DrawIndexed( const VertexBuffer& vbo,
					  const IndexBuffer& ibo ) const noexcept;

	[[nodiscard]] std::string GetDescription() const noexcept;

	void SetShader( const Shader& shader ) noexcept;
	void SetViewport( int x, int y, int width, int height ) noexcept;

	NON_COPYABLE_OR_MOVABLE_CLASS( Renderer )
};
}  // namespace Octave

#endif
