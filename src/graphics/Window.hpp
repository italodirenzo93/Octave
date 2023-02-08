#ifndef OCTAVE_WINDOW_HPP
#define OCTAVE_WINDOW_HPP

#include "CommonInclude.hpp"

namespace octave::input {
class Keyboard;
}

namespace octave::graphics {
class Window {
	friend class Renderer;
	friend class octave::input::Keyboard;

public:
	Window();
	explicit Window( const std::string& title );
	Window( int width, int height, const std::string& title );
	~Window() noexcept;

	void GetSize( int& width, int& height ) const noexcept;

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

	using OnSizeChangedCallback = std::function<void( int, int )>;
	void AddSizeChangedCallback( OnSizeChangedCallback callback ) noexcept {
		cb_window_size_.emplace_back( std::move( callback ) );
	}

private:
	GLFWwindow* handle_ = nullptr;
	bool tearing_support_ = false;

	// Callbacks
	std::vector<OnSizeChangedCallback> cb_window_size_;
	static void WindowSizeCallback( GLFWwindow* window, int width,
									int height ) noexcept;

	NON_COPYABLE_OR_MOVABLE_CLASS( Window )
};
}  // namespace octave::graphics

#endif
