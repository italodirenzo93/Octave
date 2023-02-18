#ifndef OCTAVE_WINDOW_HPP
#define OCTAVE_WINDOW_HPP

#include "CommonInclude.hpp"

namespace Octave::input {
class Keyboard;
}

namespace Octave::graphics {
class Window {
	friend class Renderer;
	friend class Octave::input::Keyboard;

public:
	Window();
	explicit Window( const std::string& title );
	Window( int width, int height, const std::string& title );
	~Window() noexcept;

	[[nodiscard]] std::pair<int, int> GetSize() const noexcept;

	[[nodiscard]] bool IsOpen() const noexcept;
	[[nodiscard]] bool IsMinimized() const noexcept;
	[[nodiscard]] bool IsMaximized() const noexcept;

	Window& SetTitle( const std::string& title ) noexcept;
	Window& SetSyncInterval( int interval ) noexcept;

	// Actions
	void PollEvents() const noexcept;
	void Minimize() const noexcept;
	void Maximize() const noexcept;
	void Restore() const noexcept;
	void Close() const noexcept;

	// Callbacks
	using OnSizeChangedCallback = std::function<void( int, int )>;
	Window& AddSizeChangedCallback( OnSizeChangedCallback callback ) noexcept {
		cb_window_size_.emplace_back( std::move( callback ) );
		return *this;
	}

	using OnCloseCallback = std::function<void( void )>;
	Window& AddCloseCallback( OnCloseCallback callback ) noexcept {
		cb_close_.emplace_back( std::move( callback ) );
		return *this;
	}

private:
	GLFWwindow* handle_ = nullptr;
	bool tearing_support_ = false;

	// Callbacks
	std::vector<OnSizeChangedCallback> cb_window_size_;
	static void WindowSizeCallback( GLFWwindow* window, int width,
									int height ) noexcept;

	std::vector<OnCloseCallback> cb_close_;
	static void CloseCallback( GLFWwindow* window ) noexcept;

	NON_COPYABLE_OR_MOVABLE_CLASS( Window )
};
}  // namespace octave::graphics

#endif
