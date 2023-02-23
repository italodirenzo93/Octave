#ifndef OCTAVE_WINDOWGLFW_HPP
#define OCTAVE_WINDOWGLFW_HPP

#include "platform/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace octave::platform::glfw {

class WindowGLFW : public Window {
public:
	explicit WindowGLFW( const WindowOptions& options );
	~WindowGLFW() noexcept override;
	[[nodiscard]] std::pair<int, int> GetSize() const noexcept override;
	[[nodiscard]] bool IsOpen() const noexcept override;
	Window& SetTitle( const std::string& title ) noexcept override;
	Window& SetSyncInterval( int interval ) noexcept override;
	void PollEvents() const noexcept override;
	void Close() const noexcept override;

private:
	GLFWwindow* window_ = nullptr;

	static void WindowSizeCallback( GLFWwindow* window, int width,
									int height ) noexcept;
	static void CloseCallback( GLFWwindow* window ) noexcept;
};

}  // namespace octave::platform::glfw

#endif  // OCTAVE_WINDOWGLFW_HPP
