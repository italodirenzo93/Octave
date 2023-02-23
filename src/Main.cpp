#include "CommonInclude.hpp"
#include "graphics/Renderer.hpp"

static constexpr int kDefaultWidth = 1280;
static constexpr int kDefaultHeight = 720;

int main() {
    GLFWwindow* window = nullptr;

    // Initialize GLFW library
    if (!glfwInit()) {
        printf( "Failed to initialize GLFW... exiting\n" );
        return EXIT_FAILURE;
    }

    // Set window hints
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
        printf( "Failed to create GLFW window... exiting\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent( window );

    // Initialize Open GL extension loader
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress) )) {
        printf(
            "Failed set proc address for Open GL extension loader... exiting\n" );
        return EXIT_FAILURE;
    }

	// Because Open GL coordinates are weird...
	stbi_set_flip_vertically_on_load( true );

	// Renderer initialization
    graphics::Initialize();

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
