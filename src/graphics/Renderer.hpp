#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Window.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

namespace octave::graphics {
enum class PrimitiveType {
    kTriangleList,
    kTriangleStrip,
    kTriangleFan
};

class Renderer {
public:
    explicit Renderer( const Window& window ) noexcept;
    ~Renderer() noexcept = default;

    void Clear( bool depth = true, float r = 0.0f, float g = 0.0f,
                float b = 0.0f, float a = 1.0f ) const noexcept;
    void Present() const noexcept;

    void DrawPrimitives( PrimitiveType type,
                         const VertexBuffer& vbo ) const noexcept;
    void DrawIndexedPrimitives( PrimitiveType type, const VertexBuffer& vbo,
                                const IndexBuffer& ibo ) const noexcept;

    [[nodiscard]] std::string GetDescription() const noexcept;
    void GetFramebufferSize( int& width, int& height ) const noexcept;

    void SetShader( const Shader& shader ) noexcept;

	// Callbacks
	using OnFramebufferSizeChangedCallback = std::function<void(int, int)>;
    void SetFramebufferSizeChangedCallback( OnFramebufferSizeChangedCallback callback ) noexcept {
        cb_framebuffer_size_callback_ = callback;
    }

private:
    const Window& window_;

	OnFramebufferSizeChangedCallback cb_framebuffer_size_callback_;
    static void FramebufferSizeCallback( GLFWwindow* window, int width,
                                         int height );

    NON_COPYABLE_OR_MOVABLE_CLASS( Renderer )
};
} // namespace octave::graphics

#endif
