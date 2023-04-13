#include "pch/pch.hpp"
#include "GLFWWindow.hpp"

#include "Config.hpp"
#include "GLFWError.hpp"
#include "core/Log.hpp"

namespace Octave {

static void SizeChangedCallback( GLFWwindow* window, int width, int height );
static void CloseCallback( GLFWwindow* window );

GLFWWindow::GLFWWindow( GLFWwindow* window ) : m_Window( window ) {
	assert( window != nullptr );

	// Pass a pointer to this class instance to the window
	// so we can access in from our callback functions
	glfwSetWindowUserPointer( window, this );

	// Set callbacks
	glfwSetWindowSizeCallback( window, SizeChangedCallback );
	glfwSetWindowCloseCallback( window, CloseCallback );
}

void* GLFWWindow::GetNativeWindowHandle() {
	return nullptr;
}

std::pair<int, int> GLFWWindow::GetSize() {
	int width, height;
	glfwGetFramebufferSize( m_Window, &width,
							&height );
	return { width, height };
}

void GLFWWindow::SetTitle( const std::string& title ) {
	glfwSetWindowTitle( m_Window, title.c_str() );
}

void GLFWWindow::SetSyncInterval( int interval ) {
	glfwSwapInterval( interval );
}

void GLFWWindow::Close() {
	glfwSetWindowShouldClose( m_Window, GLFW_TRUE );
}

void GLFWWindow::PollEvents() {
	glfwPollEvents();
}

// Callback definitions

void SizeChangedCallback( GLFWwindow* window, int width, int height ) {
	auto cb_window = static_cast<GLFWWindow*>( glfwGetWindowUserPointer( window ) );
	if ( cb_window != nullptr ) {
		cb_window->OnSizeChanged.InvokeAll( width, height );
	}
}

void CloseCallback( GLFWwindow* window ) {
	auto cb_window = static_cast<GLFWWindow*>( glfwGetWindowUserPointer( window ) );
	if ( cb_window != nullptr ) {
		cb_window->OnClose.InvokeAll();
	}
}

}  // namespace Octave
