#include "Window.hpp"

#include "Config.hpp"
#include "Exception.hpp"

using namespace std;

namespace octave::graphics {

Window::Window() : Window( "My Game Engine" ) {
}

Window::Window( const std::string& title )
    : Window( Config::Instance().GetFramebufferWidth(),
              Config::Instance().GetFramebufferHeight(), title ) {
}

Window::Window( int width, int height, const string& title ) {
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

    const Config& config = Config::Instance();

    // Fullscreen
    GLFWmonitor* monitor = nullptr;
    if ( config.IsFullscreen() ) {
        monitor = glfwGetPrimaryMonitor();

        const auto vidmode = glfwGetVideoMode( monitor );
        width = vidmode->width;
        height = vidmode->height;

        glfwWindowHint( GLFW_RED_BITS, vidmode->redBits );
        glfwWindowHint( GLFW_GREEN_BITS, vidmode->greenBits );
        glfwWindowHint( GLFW_BLUE_BITS, vidmode->blueBits );
        glfwWindowHint( GLFW_REFRESH_RATE, vidmode->refreshRate );
    }

    if ( config.IsMaximized() ) {
        glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    }

    // Create window
    handle_ =
        glfwCreateWindow( width, height, title.c_str(), monitor, nullptr );
    if ( !handle_ ) {
        throw Exception( "Unable to create GLFW window" );
    }

    glfwMakeContextCurrent( handle_ );
    glfwSwapInterval( config.GetSyncInterval() );

    // Pass a pointer to this class instance to the window
    // so we can access in from our callback functions
    glfwSetWindowUserPointer( handle_, this );

    // Set callback wrapper functions
    glfwSetWindowSizeCallback( handle_, WindowSizeCallback );

    // Initialize Open GL extension loader
    if ( !gladLoadGLLoader(
             reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) ) {
        throw Exception( "Unable to initialize GLAD OpenGL extension loader" );
    }
}

Window::~Window() noexcept {
    glfwDestroyWindow( handle_ );
}

void Window::PollEvents() const noexcept {
    glfwPollEvents();
}

bool Window::IsOpen() const noexcept {
    return !glfwWindowShouldClose( handle_ );
}

bool Window::IsMinimized() const noexcept {
    return glfwGetWindowAttrib( handle_, GLFW_ICONIFIED ) == GLFW_TRUE;
}

bool Window::IsMaximized() const noexcept {
    return glfwGetWindowAttrib( handle_, GLFW_MAXIMIZED ) == GLFW_TRUE;
}

void Window::Minimize() const noexcept {
    glfwIconifyWindow( handle_ );
}

void Window::Maximize() const noexcept {
    glfwMaximizeWindow( handle_ );
}

void Window::Restore() const noexcept {
    glfwRestoreWindow( handle_ );
}

void Window::GetSize( int& width, int& height ) const noexcept {
    glfwGetWindowSize( handle_, &width, &height );
}

void Window::WindowSizeCallback( GLFWwindow* window, int width,
                                 int height ) noexcept {
    const auto c_window =
        reinterpret_cast<Window*>( glfwGetWindowUserPointer( window ) );
    if ( c_window && c_window->cb_window_size_ ) {
        c_window->cb_window_size_( width, height );
    }
}

}  // namespace octave::graphics
