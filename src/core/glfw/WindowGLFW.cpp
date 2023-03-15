#include "pch/pch.hpp"
#include "WindowGLFW.hpp"

#include <GLFW/glfw3.h>

#include "Config.hpp"
#include "GLFWError.hpp"
#include "core/Log.hpp"

namespace Octave::Impl {

static uint32_t g_window_count = 0;

static void ErrorCallback( int error_code, const char* description ) {
	Log::GetCoreLogger()->error( "GLFW Error {} - {}", error_code, description );
}

WindowGLFW::WindowGLFW( const WindowOptions& options ) {
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

#ifdef OGT_RHI_OPENGL45

	// Open GL context hints
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
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
	window_ = glfwCreateWindow( width, height, options.title.c_str(), monitor,
								nullptr );
	if ( !window_ ) {
		throw GLFWError();
	}

	// Increment window count
	g_window_count += 1;

	glfwMakeContextCurrent( window_ );
	glfwSwapInterval( Config::Instance().GetSyncInterval() );

	// Pass a pointer to this class instance to the window
	// so we can access in from our callback functions
	glfwSetWindowUserPointer( window_, this );

	// Set callback wrapper functions
	glfwSetFramebufferSizeCallback( window_, WindowSizeCallback );
	glfwSetWindowCloseCallback( window_, CloseCallback );
}

WindowGLFW::~WindowGLFW() noexcept {
	Log::GetCoreLogger()->trace( "Destroying GLFW window" );
	glfwDestroyWindow( window_ );
	g_window_count -= 1;

	// Quit GLFW if no windows left
	if ( g_window_count == 0 ) {
		Log::GetCoreLogger()->trace( "Shutting down GLFW" );
		glfwTerminate();
	}
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

void WindowGLFW::Close() const noexcept {
	glfwSetWindowShouldClose( window_, GLFW_TRUE );
}

void WindowGLFW::PollEvents() noexcept {
	glfwPollEvents();
}

void WindowGLFW::SwapBuffers() noexcept {
	glfwSwapBuffers( window_ );
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

}  // namespace Octave::Impl
