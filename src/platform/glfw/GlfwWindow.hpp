#ifndef OCTAVE_GLFWWINDOW_HPP
#define OCTAVE_GLFWWINDOW_HPP

#include <GLFW/glfw3.h>

#include "platform/Window.hpp"

namespace octave::platform::glfw {

class GLFWWindow : public Window {
public:
	GLFWWindow( int width, int height, const std::string& title );
	[[nodiscard]] std::pair<int, int> GetSize() const noexcept override;
	[[nodiscard]] bool IsOpen() const noexcept override;
	Window& SetTitle( const std::string& title ) noexcept override;
	Window& SetSyncInterval( int interval ) noexcept override;
	void PollEvents() const noexcept override;
	void Close() const noexcept override;

private:
	::GLFWwindow* window_ = nullptr;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_GLFWWINDOW_HPP
