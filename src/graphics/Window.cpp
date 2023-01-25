#include "Window.hpp"

#include "Config.hpp"

using namespace std;

namespace graphics {

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

    GLFWmonitor* monitor = nullptr;
    if ( config.GetIsFullscreen() ) {
        monitor = glfwGetPrimaryMonitor();
    }

    // Create window
    handle_ =
        glfwCreateWindow( width, height, title.c_str(), monitor, nullptr );
    if ( !handle_ ) {
        cout << "Failed to create GLFW window..." << endl;
        return;
    }

    glfwMakeContextCurrent( handle_ );
    glfwSwapInterval( config.GetSyncInterval() );

    glfwSetFramebufferSizeCallback( handle_,
                                    []( GLFWwindow*, int width, int height ) {
                                        glViewport( 0, 0, width, height );
                                    } );

    // Initialize Open GL extension loader
    if ( !gladLoadGLLoader(
             reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) ) {
        cout
            << "Failed set proc address for Open GL extension loader... exiting"
            << endl;
        return;
    }
}

Window::~Window() {
    glfwDestroyWindow( handle_ );
}

void Window::PollEvents() const {
    glfwPollEvents();
}

bool Window::IsOpen() const {
    return !glfwWindowShouldClose( handle_ );
}

void Window::GetSize( int& width, int& height ) const {
    glfwGetWindowSize( handle_, &width, &height );
}

}  // namespace graphics
