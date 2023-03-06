#ifndef OCTAVE_GRAPHICS_OPENGL_GRAPHICSCONTEXTGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_GRAPHICSCONTEXTGL_HPP

#include "graphics/GraphicsContext.hpp"
#include "BufferGL.hpp"
#include "VertexArrayLayoutGL.hpp"

namespace Octave::Impl {

class GraphicsContextGL : public GraphicsContext {
public:
	GraphicsContextGL();
	virtual ~GraphicsContextGL() noexcept override;

	void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f,
				float a = 1.0f ) const noexcept override;

	void Draw( size_t vertex_count, size_t offset ) const noexcept override;
	void DrawIndexed( size_t index_count, size_t offset, size_t base_vertex ) const noexcept override;

	[[nodiscard]] std::array<int, 4> GetViewport() const noexcept override;

	void SetDepthTestEnabled( bool enabled ) noexcept override;
	void SetVertexBuffer( SharedRef<Buffer> vertex_buffer, size_t stride, SharedRef<VertexArrayLayout> layout ) override;
	void SetIndexBuffer( SharedRef<Buffer> index_buffer ) override;
	void SetVertexShader( SharedRef<Shader> vertex_shader ) override;
	void SetFragmentShader( SharedRef<Shader> fragment_shader ) override;
	void SetViewport( int x, int y, int width, int height ) noexcept override;

private:
	uint32_t pipeline_id_ = 0;
	SharedRef<BufferGL> ibo_;
	SharedRef<VertexArrayLayoutGL> vao_;
	uint32_t vertex_stride_ = 0;
};

}  // namespace Octave::Impl

#endif
