#include "Renderer.hpp"
#include "ShaderManager.hpp"
#include "Config.hpp"

#include <sstream>

namespace graphics {
using namespace std;

#ifdef __DEBUG__
void APIENTRY DebugOutputCallback( GLenum source, GLenum type, unsigned int id,
                                   GLenum severity, GLsizei length,
                                   const char* message,
                                   const void* userParam ) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

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

// Default width, height
static constexpr int kDefaultWidth = 1280;
static constexpr int kDefaultHeight = 720;

// Open GL window
static GLFWwindow* g_window = nullptr;

bool Initialize() {
    // Initialize GLFW library
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW... exiting" << endl;
        return false;
    }

    // Set window hints
#ifdef __APPLE__
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#else
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#endif
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#ifdef __DEBUG__
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif

    int width, height;
    if (!config::TryGetInt( "Video", "FramebufferWidth", width )) {
        width = kDefaultWidth;
    }
    if (!config::TryGetInt( "Video", "FramebufferHeight", height )) {
        height = kDefaultHeight;
    }

    int sync_interval;
    if (!config::TryGetInt( "Video", "SyncInterval", sync_interval )) {
        sync_interval = 1;
    }

    // Create window
    g_window =
        glfwCreateWindow( width, height, "My Game Engine", nullptr, nullptr );
    if (!g_window) {
        cout << "Failed to create GLFW window... exiting" << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent( g_window );
    glfwSwapInterval( sync_interval );

    glfwSetFramebufferSizeCallback( g_window,
                                    []( GLFWwindow*, int width, int height ) {
                                        glViewport( 0, 0, width, height );
                                    } );

    // Initialize Open GL extension loader
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress) )) {
        cout
            << "Failed set proc address for Open GL extension loader... exiting"
            << endl;
        return EXIT_FAILURE;
    }

    // Always enabled
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    glClearDepthf( 1.0f );

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
    {
        bool preload = false;
        config::TryGetBool( "Renderer", "PreloadShaders", preload );

        if (preload) {
            ShaderManager::Instance().PreloadShaders();
        }
    }

    return true;
}

void Terminate() {
    if (g_window) {
        glfwDestroyWindow( g_window );
    }
    glfwTerminate();
}

void ResizeFramebuffer( int width, int height ) {
    glViewport( 0, 0, width, height );
}

bool IsWindowOpen() {
    return g_window != nullptr && !glfwWindowShouldClose( g_window );
}

void Clear( bool depth, float r, float g, float b, float a ) {
    int clear_flags = GL_COLOR_BUFFER_BIT;

    if (depth) {
        clear_flags |= GL_DEPTH_BUFFER_BIT;
    }

    glClearColor( r, g, b, a );
    glClear( clear_flags );
}

void Present() {
    glfwPollEvents();
    if (g_window) {
        glfwSwapBuffers( g_window );
    }
}

std::string GetRendererInfo() {
    ostringstream oss;

    // Print OpenGL context information
    oss << "OpenGL Context Version: " << glGetString( GL_VERSION ) << endl
        << "GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION )
        << endl
        << "GPU Vendor: " << glGetString( GL_VENDOR ) << endl
        << "GPU Model: " << glGetString( GL_RENDERER ) << endl;

    return oss.str();
}
}
