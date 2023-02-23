#include "WindowGLFW.hpp"

#include "Config.hpp"
#include "GLFWError.hpp"

namespace octave::platform::glfw {

WindowGLFW::WindowGLFW( const WindowOptions& options ) : Window( options ) {
	// Set defaults as a baseline
	glfwDefaultWindowHints();

	// Open GL context hints
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );

	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );

#ifdef OCTAVE_DEBUG
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif

	const Config& config = Config::Instance();

	// Fullscreen
	GLFWmonitor* monitor = nullptr;

	int width = options.width;
	int height = options.height;

	if ( options.is_fullscreen ) {
		const auto primary_monitor = glfwGetPrimaryMonitor();
		const auto vidmode = glfwGetVideoMode( primary_monitor );

		width = vidmode->width;
		height = vidmode->height;

		glfwWindowHint( GLFW_RED_BITS, vidmode->redBits );
		glfwWindowHint( GLFW_GREEN_BITS, vidmode->greenBits );
		glfwWindowHint( GLFW_BLUE_BITS, vidmode->blueBits );
		glfwWindowHint( GLFW_REFRESH_RATE, vidmode->refreshRate );

		monitor = primary_monitor;  // passing a monitor to glfwCreateWindow
									// enables fullscreen mode
	}

	if ( options.is_borderless ) {
		glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
		monitor = nullptr;  // window is the size of the monitor but not true
							// fullscreen
	}

	// Create window
	window_ = glfwCreateWindow( width, height, options.title.c_str(), monitor,
								nullptr );
	if ( !window_ ) {
		throw GLFWError();
	}

	glfwMakeContextCurrent( window_ );

	SetSyncInterval( config.GetSyncInterval() );

	// Pass a pointer to this class instance to the window
	// so we can access in from our callback functions
	glfwSetWindowUserPointer( window_, this );

	// Set callback wrapper functions
	glfwSetFramebufferSizeCallback( window_, WindowSizeCallback );
	glfwSetWindowCloseCallback( window_, CloseCallback );

	// Initialize Open GL extension loader
	if ( !gladLoadGLLoader(
			 reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) ) {
		throw Exception( "Unable to initialize GLAD OpenGL extension loader" );
	}
}

WindowGLFW::~WindowGLFW() noexcept {
	glfwDestroyWindow( window_ );
}

std::pair<int, int> WindowGLFW::GetSize() const noexcept {
	int width, height;
	glfwGetFramebufferSize( window_, &width, &height );
	return { width, height };
}

bool WindowGLFW::IsOpen() const noexcept {
	return !glfwWindowShouldClose( window_ );
}

Window& WindowGLFW::SetTitle( const std::string& title ) noexcept {
	glfwSetWindowTitle( window_, title.c_str() );
	return *this;
}

Window& WindowGLFW::SetSyncInterval( int interval ) noexcept {
	glfwSwapInterval( interval );
	return *this;
}

void WindowGLFW::PollEvents() const noexcept {
	glfwPollEvents();
}

void WindowGLFW::Close() const noexcept {
	glfwSetWindowShouldClose( window_, GLFW_TRUE );
}

void WindowGLFW::WindowSizeCallback( GLFWwindow* window, int width,
									 int height ) noexcept {
	const auto c_window =
		static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );
	if ( c_window ) {
		for ( const auto& callback : c_window->cb_window_size_ ) {
			callback( width, height );
		}
	}
}

void WindowGLFW::CloseCallback( GLFWwindow* window ) noexcept {
	const auto c_window =
		static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );
	if ( c_window ) {
		for ( const auto& callback : c_window->cb_close_ ) {
			callback();
		}
	}
}

}  // namespace octave::platform::glfw
