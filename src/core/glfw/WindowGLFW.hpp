#ifndef OCTAVE_WINDOWGLFW_HPP
#define OCTAVE_WINDOWGLFW_HPP

#include "core/Window.hpp"

struct GLFWwindow;

namespace Octave::Impl {

class WindowGLFW : public Window {
public:
	explicit WindowGLFW( const WindowOptions& options );
	~WindowGLFW() noexcept override;

	[[nodiscard]] void* GetNativeWindowHandle() const noexcept override {
		return window_;
	};
	[[nodiscard]] std::pair<int, int> GetSize() const noexcept override;
	[[nodiscard]] GLFWwindow* GetPointer() const noexcept { return window_; }
	[[nodiscard]] bool IsOpen() const noexcept override;

	Window& SetTitle( const std::string& title ) noexcept override;
	Window& SetSyncInterval( int interval ) noexcept override;

	void Close() const noexcept override;
	void PollEvents() noexcept override;
	void SwapBuffers() noexcept override;

private:
	GLFWwindow* window_ = nullptr;

	static void WindowSizeCallback( GLFWwindow* window, int width,
									int height ) noexcept;
	static void CloseCallback( GLFWwindow* window ) noexcept;
};

}  // namespace Octave::Impl

#endif  // OCTAVE_WINDOWGLFW_HPP
