#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "CommonInclude.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

namespace graphics {

class Renderer {
   public:
    Renderer();
    ~Renderer();

    void ResizeFramebuffer( int width, int height ) const;
    bool IsWindowOpen() const;
    std::string GetDescription() const;

    void Clear( bool depth = true, float r = 0.0f, float g = 0.0f,
                float b = 0.0f, float a = 1.0f ) const;
    void Present() const;

    void DrawPrimitives( const VertexArrayLayout& vao,
                         const VertexBuffer& vbo ) const;
    void DrawIndexedPrimitives( const VertexArrayLayout& vao,
                                const IndexBuffer& ibo ) const;

   private:
    GLFWwindow* window_ = nullptr;

    NON_COPYABLE_OR_MOVABLE_CLASS( Renderer )
};

}  // namespace graphics

#endif
