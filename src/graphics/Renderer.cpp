#include "Renderer.hpp"

#include <sstream>

#include "Config.hpp"
#include "ShaderManager.hpp"

namespace graphics {
using namespace std;

inline GLenum PrimitiveToGLType( PrimitiveType type ) {
    switch (type) {
        case PrimitiveType::kTriangleList:
            return GL_TRIANGLES;
        case PrimitiveType::kTriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::kTriangleFan:
            return GL_TRIANGLE_FAN;
    }

    return 0;
}

#ifdef __DEBUG__
void APIENTRY DebugOutputCallback( GLenum source, GLenum type, unsigned int id,
                                   GLenum severity, GLsizei length,
                                   const char* message,
                                   const void* userParam ) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
#endif

Renderer::Renderer( const Window& window ) noexcept
    : window_( window ) {
    // Always enabled
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    glClearDepthf( 1.0f );
    glCullFace( GL_BACK );

#ifdef __DEBUG__
    // Configure debug callback if we got a debug context
    int gl_context_flags;
    glGetIntegerv( GL_CONTEXT_FLAGS, &gl_context_flags );
    if (gl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
        glDebugMessageCallback( DebugOutputCallback, nullptr );
        glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                               nullptr, GL_TRUE );
    }
#endif

    // Because Open GL coordinates are weird...
    stbi_set_flip_vertically_on_load( true );

    // Shader pre-cache
    if (Config::Instance().GetPreloadShaders()) {
        ShaderManager::Instance().PreloadShaders();
    }
}

void Renderer::ResizeFramebuffer( int width, int height ) const {
    glViewport( 0, 0, width, height );
}

void Renderer::Clear( bool depth, float r, float g, float b, float a ) const noexcept {
    int clear_flags = GL_COLOR_BUFFER_BIT;

    if (depth) {
        clear_flags |= GL_DEPTH_BUFFER_BIT;
        glClearDepthf( 1.0f );
    }

    glClearColor( r, g, b, a );
    glClear( clear_flags );
}

void Renderer::Present() const noexcept {
    glfwSwapBuffers( window_.handle_ );
}

void Renderer::DrawPrimitives( PrimitiveType type,
                               const VertexBuffer& vbo ) const noexcept {
    glBindVertexArray( vbo.vao_ );

    glDrawArrays( PrimitiveToGLType( type ), 0,
                  static_cast<int>(vbo.GetVertexCount()) );

    glBindVertexArray( 0 );
}

void Renderer::DrawIndexedPrimitives( PrimitiveType type,
                                      const VertexBuffer& vbo,
                                      const IndexBuffer& ibo ) const noexcept {
    glBindVertexArray( vbo.vao_ );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo.id_ );

    glDrawElements( PrimitiveToGLType( type ),
                    static_cast<int>(ibo.GetElementCount()),
                    GL_UNSIGNED_INT, nullptr );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

std::string Renderer::GetDescription() const noexcept {
    ostringstream oss;

    // Print OpenGL context information
    oss << "OpenGL Context Version: " << glGetString( GL_VERSION ) << endl
        << "GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION )
        << endl
        << "GPU Vendor: " << glGetString( GL_VENDOR ) << endl
        << "GPU Model: " << glGetString( GL_RENDERER ) << endl;

    return oss.str();
}

void Renderer::GetFramebufferSize( int& width, int& height ) const noexcept {
    glfwGetFramebufferSize( window_.handle_, &width, &height );
}

void Renderer::SetTexture( const Texture& texture ) noexcept {
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture.id_ );
}

void Renderer::SetShader( const Shader& shader ) noexcept {
    glUseProgram( shader.id_ );
}
} // namespace graphics
