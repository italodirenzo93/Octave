#ifndef OCTAVE_RENDERERGL_HPP
#define OCTAVE_RENDERERGL_HPP

#include "graphics/Renderer.hpp"

namespace Octave::Impl {

class RendererGL : public Renderer {
public:
	RendererGL();
	virtual ~RendererGL() noexcept override;

	void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f,
				float a = 1.0f ) const noexcept override;

	void Draw( const Shader& shader,
			   const VertexBuffer& vbo ) const noexcept override;
	void DrawIndexed( const Shader& shader, const VertexBuffer& vbo,
					  const IndexBuffer& ibo ) const noexcept override;

	[[nodiscard]] std::string GetDescription() const noexcept override;

	void SetDepthTestEnabled( bool enabled ) noexcept override;
	void SetViewport( int x, int y, int width, int height ) noexcept override;
};

}  // namespace Octave::Impl

#endif
