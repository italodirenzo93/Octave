#ifndef OCTAVE_CORE_GLFW_GLFWWINDOW_HPP
#define OCTAVE_CORE_GLFW_GLFWWINDOW_HPP

#include "core/Window.hpp"

#include <GLFW/glfw3.h>

namespace Octave {

class GLFWWindow final : public Window {
public:
	explicit GLFWWindow( GLFWwindow* window );

	[[nodiscard]] void* GetNativeWindowHandle() override;
	[[nodiscard]] std::pair<int, int> GetSize() override;
	[[nodiscard]] GLFWwindow* GetGlfwWindowPointer() const noexcept { return m_Window; }

	void SetTitle( const std::string& title ) override;
	void SetSyncInterval( int interval ) override;

	void Close() override;
	void PollEvents() override;

private:
	GLFWwindow* m_Window;
};

}

#endif
