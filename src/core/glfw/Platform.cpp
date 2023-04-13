#include "Config.hpp"
#include "core/Platform.hpp"
#include "core/Log.hpp"

#include <GLFW/glfw3.h>

#include "GLFWWindow.hpp"

namespace Octave {

static bool g_initialized = false;

static void ErrorCallback( int error_code, const char* description ) {
	Log::GetCoreLogger()->error( "GLFW Error {} - {}", error_code,
								 description );
}

bool Platform::Init() {
	if ( g_initialized ) return true;

	Log::GetCoreLogger()->trace( "Initializing platform module" );

	if ( !glfwInit() ) {
		Log::GetCoreLogger()->error( "Failed to initialize GLFW" );
		return false;
	}

	glfwSetErrorCallback( ErrorCallback );

	g_initialized = true;
	return true;
}

void Platform::Quit() {
	if ( !g_initialized ) return;

	Log::GetCoreLogger()->trace( "Shutting down platform module" );
	glfwTerminate();
	g_initialized = false;
}

PlatformName Platform::GetName() noexcept {
#if defined( OGT_PLATFORM_WINDOWS )
	return PlatformName::Windows;
#elif defined( OGT_PLATFORM_MACOS )
	return PlatformName::Mac;
#elif defined( OGT_PLATFORM_LINUX )
	return PlatformName::Linux;
#else
#error "Unsupported platform"
#endif
}

double Platform::GetElapsedTime() noexcept {
	return glfwGetTime();
}

uint64_t Platform::GetTimerFrequency() noexcept {
	return glfwGetTimerFrequency();
}

uint64_t Platform::GetTimerValue() noexcept {
	return glfwGetTimerValue();
}

std::unique_ptr<Window> Platform::CreateWindow( const WindowOptions& options ) {
	Log::GetCoreLogger()->trace( "Creating GLFW window {}x{} Title: {}", options.width, options.height, options.title );

	// Set defaults as a baseline
	glfwDefaultWindowHints();

#ifdef OGT_RHI_OPENGL

	// Open GL context hints
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
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
		const char* error;
		glfwGetError( &error );

		Log::GetCoreLogger()->error( "Unable to create GLFW window: {} ", error );

		return nullptr;
	}

	glfwMakeContextCurrent( window );
	glfwSwapInterval( Config::Instance().GetSyncInterval() );

	return std::make_unique<GLFWWindow>( window );
}

void Platform::DestroyWindow( std::unique_ptr<Window> window ) {
	assert( window != nullptr );
	Log::GetCoreLogger()->trace( "Destroying GLFW window" );
	glfwDestroyWindow( dynamic_cast<GLFWWindow*>( window.get() )->GetGlfwWindowPointer() );
}

}  // namespace Octave
