#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Window.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

namespace graphics {
class Renderer {
public:
    explicit Renderer( const Window& window );
    ~Renderer() = default;

    void ResizeFramebuffer( int width, int height ) const;

    void Clear( bool depth = true, float r = 0.0f, float g = 0.0f,
                float b = 0.0f, float a = 1.0f ) const;
    void Present() const;

    void DrawPrimitives( const VertexArrayLayout& vao,
                         const VertexBuffer& vbo ) const;
    void DrawIndexedPrimitives( const VertexArrayLayout& vao,
                                const IndexBuffer& ibo ) const;

    std::string GetDescription() const;
    void GetFramebufferSize( int& width, int& height ) const;

    void SetTexture( const Texture& texture );
    void SetShader( const Shader& shader );

private:
    const Window& window_;

    NON_COPYABLE_OR_MOVABLE_CLASS( Renderer )
};
} // namespace graphics

#endif
