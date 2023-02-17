#include "GlfwWindow.hpp"

namespace octave::platform::glfw {

GLFWWindow::GLFWWindow( int width, int height, const std::string& title )
	: Window( width, height, title ) {



}

std::pair<int, int> GLFWWindow::GetSize() const noexcept {
	int width, height;
	glfwGetFramebufferSize( window_, &width, &height );
	return { width, height };
}

bool GLFWWindow::IsOpen() const noexcept {
	return !glfwWindowShouldClose( window_ );
}

Window& GLFWWindow::SetTitle( const std::string& title ) noexcept {
	glfwSetWindowTitle( window_, title.c_str() );
	return *this;
}

Window& GLFWWindow::SetSyncInterval( int interval ) noexcept {
	glfwSwapInterval( interval );
	return *this;
}

void GLFWWindow::PollEvents() const noexcept {
	glfwPollEvents();
}

void GLFWWindow::Close() const noexcept {
	glfwSetWindowShouldClose( window_, GLFW_TRUE );
}

}  // namespace octave::platform::glfw
