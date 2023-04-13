#ifndef OCTAVE_GRAPHICS_OPENGL_OPENGLGRAPHICSCONTEXT_HPP
#define OCTAVE_GRAPHICS_OPENGL_OPENGLGRAPHICSCONTEXT_HPP

#include "pch/pch.hpp"
#include "graphics/GraphicsContext.hpp"

#include <glad/glad.h>

namespace Octave {

class OpenGLGraphicsContext final : public GraphicsContext {
public:
	OpenGLGraphicsContext() noexcept;

	[[nodiscard]] GLuint GetApiResource() const noexcept { return m_Vao; }
	void SetApiResource( GLuint resource ) noexcept { m_Vao = resource; }

	void Reset() noexcept override;
	void Clear( bool color = true, bool depth = true, float r = 0.0f,
				float g = 0.0f, float b = 0.0f,
				float a = 1.0f ) noexcept override;

	void Draw( size_t vertex_count, size_t offset ) noexcept override;
	void DrawIndexed( size_t index_count, size_t offset,
					  size_t base_vertex ) noexcept override;

	[[nodiscard]] std::array<int, 4> GetViewport() const noexcept override;

	void SetProgram( const Program& program ) override;
	void SetVertexBuffer( const Buffer& vbo ) override;
	void SetIndexBuffer( const Buffer& ibo ) override;
	void SetSampler( uint32_t unit, const Sampler& sampler ) override;
	void SetTexture( uint32_t unit, const Texture2D& texture ) override;
	void SetVertexLayout( const VertexLayout& layout ) override;
	void SetViewport( int x, int y, int width, int height ) override;

private:
	GLuint m_Vao;
	VertexLayout m_VertexLayout;
	GLuint m_Vbo, m_Ibo, m_Program;
	std::vector<GLuint> m_Textures;
	std::vector<GLuint> m_Samplers;
	GLsizei m_VertexStride;
	GLint m_MaxIndices;
	GLint m_MaxTextures;
	void PrepareToDraw();
};

}  // namespace Octave::OpenGL

#endif
