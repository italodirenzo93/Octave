#include "pch/pch.hpp"
#include "Window.hpp"

#include <GLFW/glfw3.h>

#include "Config.hpp"
#include "GLFWError.hpp"
#include "core/Log.hpp"

namespace Octave {

static uint32_t g_window_count = 0;

static void ErrorCallback( int error_code, const char* description );
static void SizeChangedCallback( GLFWwindow* window, int width, int height );
static void CloseCallback( GLFWwindow* window );

Window::Window( const WindowOptions& options ) {
	// Init GLFW
	if ( g_window_count == 0 ) {
		Log::GetCoreLogger()->trace( "Initializing GLFW" );
		if ( !glfwInit() ) {
			throw Exception( "Unable to initialize GLFW" );
		}
		glfwSetErrorCallback( ErrorCallback );
	}

	Log::GetCoreLogger()->trace( "Creating GLFW window {}x{} Title: {}", options.width, options.height, options.title );

	// Set defaults as a baseline
	glfwDefaultWindowHints();

#ifdef OGT_RHI_OPENGL46

	// Open GL context hints
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#ifdef OGT_DEBUG
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif

#else

#error Unsupported RHI

#endif

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
	const auto window = glfwCreateWindow( width, height, options.title.c_str(), monitor,
								nullptr );
	if ( !window ) {
		throw GLFWError();
	}

	// Save window handle reference
	handle_ = window;

	// Increment window count
	g_window_count += 1;

	glfwMakeContextCurrent( window );
	glfwSwapInterval( Config::Instance().GetSyncInterval() );

	// Pass a pointer to this class instance to the window
	// so we can access in from our callback functions
	glfwSetWindowUserPointer( window, this );

	// Set callbacks
	glfwSetWindowSizeCallback( window, SizeChangedCallback );
	glfwSetWindowCloseCallback( window, CloseCallback );
}

Window::~Window() noexcept {
	Log::GetCoreLogger()->trace( "Destroying GLFW window" );
	glfwDestroyWindow( static_cast<GLFWwindow*>( handle_ ) );
	g_window_count -= 1;

	// Quit GLFW if no windows left
	if ( g_window_count == 0 ) {
		Log::GetCoreLogger()->trace( "Shutting down GLFW" );
		glfwTerminate();
	}
}

std::pair<int, int> Window::GetSize() const noexcept {
	int width, height;
	glfwGetFramebufferSize( static_cast<GLFWwindow*>( handle_ ), &width,
							&height );
	return { width, height };
}

bool Window::IsOpen() const noexcept {
	return !glfwWindowShouldClose( static_cast<GLFWwindow*>( handle_ ) );
}

Window& Window::SetTitle( const std::string& title ) noexcept {
	glfwSetWindowTitle( static_cast<GLFWwindow*>( handle_ ), title.c_str() );
	return *this;
}

Window& Window::SetSyncInterval( int interval ) noexcept {
	glfwSwapInterval( interval );
	return *this;
}

void Window::Close() const noexcept {
	glfwSetWindowShouldClose( static_cast<GLFWwindow*>( handle_ ), GLFW_TRUE );
}

void Window::PollEvents() noexcept {
	glfwPollEvents();
}

void Window::SwapBuffers() noexcept {
	glfwSwapBuffers( static_cast<GLFWwindow*>( handle_ ) );
}

void ErrorCallback( int error_code, const char* description ) {
	Log::GetCoreLogger()->error( "GLFW Error {} - {}", error_code,
								 description );
}

// Callback definitions

void SizeChangedCallback( GLFWwindow* window, int width, int height ) {
	auto cb_window = static_cast<Window*>( glfwGetWindowUserPointer( window ) );
	if ( cb_window != nullptr ) {
		cb_window->OnSizeChanged.InvokeAll( width, height );
	}
}

void CloseCallback( GLFWwindow* window ) {
	auto cb_window = static_cast<Window*>( glfwGetWindowUserPointer( window ) );
	if ( cb_window != nullptr ) {
		cb_window->OnClose.InvokeAll();
	}
}

}  // namespace Octave
