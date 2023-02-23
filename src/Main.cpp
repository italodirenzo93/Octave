#include "CommonInclude.hpp"
#include "graphics/Renderer.hpp"

using namespace std;

static constexpr int kDefaultWidth = 1280;
static constexpr int kDefaultHeight = 720;

int main( int argc, char* argv[] ) {
    GLFWwindow* window = nullptr;

    // Initialize GLFW library
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW... exiting" << endl;
        return EXIT_FAILURE;
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

    // Create window
    window = glfwCreateWindow( kDefaultWidth, kDefaultHeight, "My Game Engine",
                               nullptr, nullptr );
    if (!window) {
        cout << "Failed to create GLFW window... exiting" << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent( window );

    // Initialize Open GL extension loader
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress) )) {
        cout
            << "Failed set proc address for Open GL extension loader... exiting"
            << endl;
        return EXIT_FAILURE;
    }

    // Renderer initialization
    graphics::Initialize();
    cout << graphics::GetRendererInfo() << endl;

    glfwSetFramebufferSizeCallback(
        window, []( GLFWwindow*, int width, int height ) {
            graphics::ResizeFramebuffer( width, height );
        } );

    // Main loop
    while (!glfwWindowShouldClose( window )) {
        graphics::Clear( true, 0.1f, 0.1f, 0.1f );

        glfwPollEvents();
        glfwSwapBuffers( window );
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
