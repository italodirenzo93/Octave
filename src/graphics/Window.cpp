#include "Window.hpp"

#include "Config.hpp"
#include "Exception.hpp"

#ifdef max
#undef max
#endif

using namespace std;

namespace octave::graphics {

#ifdef OCTAVE_DEBUG
static void GLAPIENTRY DebugOutputCallback( GLenum source, GLenum type,
											unsigned int id, GLenum severity,
											GLsizei length, const char* message,
											const void* userParam ) {
	// ignore non-significant error/warning codes
	if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch ( source ) {
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

	switch ( type ) {
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

	switch ( severity ) {
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

Window::Window() : Window( "Octave GT" ) {
}

Window::Window( const std::string& title )
	: Window( Config::Instance().GetFramebufferWidth(),
			  Config::Instance().GetFramebufferHeight(), title ) {
}

Window::Window( int width, int height, const string& title ) {
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

	if ( config.IsFullscreen() ) {
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

	if ( config.IsBorderless() ) {
		glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
		monitor = nullptr;  // window is the size of the monitor but not true
							// fullscreen
	}

	// Create window
	handle_ =
		glfwCreateWindow( width, height, title.c_str(), monitor, nullptr );
	if ( !handle_ ) {
		const char* description = nullptr;
		glfwGetError( &description );
		throw Exception( description );
	}

	glfwMakeContextCurrent( handle_ );

	tearing_support_ = glfwExtensionSupported( "WGL_EXT_swap_control_tear" ) ||
					   glfwExtensionSupported( "GLX_EXT_swap_control_tear" );
	SetSyncInterval( config.GetSyncInterval() );

	// Pass a pointer to this class instance to the window
	// so we can access in from our callback functions
	glfwSetWindowUserPointer( handle_, this );

	// Set callback wrapper functions
	glfwSetFramebufferSizeCallback( handle_, WindowSizeCallback );
	glfwSetWindowCloseCallback( handle_, CloseCallback );

	// Initialize Open GL extension loader
	if ( !gladLoadGLLoader(
			 reinterpret_cast<GLADloadproc>( glfwGetProcAddress ) ) ) {
		throw Exception( "Unable to initialize GLAD OpenGL extension loader" );
	}

#ifdef OCTAVE_DEBUG
	// Configure debug callback if we got a debug context
	int gl_context_flags;
	glGetIntegerv( GL_CONTEXT_FLAGS, &gl_context_flags );
	if ( gl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT ) {
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( DebugOutputCallback, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE,
							   GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
							   GL_FALSE );
	}
#endif
}

Window::~Window() noexcept {
	glfwDestroyWindow( handle_ );
}

std::pair<int, int> Window::GetSize() const noexcept {
	int width, height;
	glfwGetWindowSize( handle_, &width, &height );
	return { width, height };
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

Window& Window::SetTitle( const std::string& title ) noexcept {
	glfwSetWindowTitle( handle_, title.c_str() );
	return *this;
}

Window& Window::SetSyncInterval( int interval ) noexcept {
	if ( !tearing_support_ ) {
		interval = std::max( interval, 0 );
	}
	glfwSwapInterval( interval );
	return *this;
}

void Window::PollEvents() const noexcept {
	glfwPollEvents();
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

void Window::Close() const noexcept {
	glfwSetWindowShouldClose( handle_, GLFW_TRUE );
}

void Window::WindowSizeCallback( GLFWwindow* window, int width,
								 int height ) noexcept {
	const auto c_window =
		static_cast<Window*>( glfwGetWindowUserPointer( window ) );
	if ( c_window ) {
		for ( const auto& callback : c_window->cb_window_size_ ) {
			callback( width, height );
		}
	}
}

void Window::CloseCallback( GLFWwindow* window ) noexcept {
	const auto c_window =
		static_cast<Window*>( glfwGetWindowUserPointer( window ) );
	if ( c_window ) {
		for ( const auto& callback : c_window->cb_close_ ) {
			callback();
		}
	}
}

}  // namespace octave::graphics
