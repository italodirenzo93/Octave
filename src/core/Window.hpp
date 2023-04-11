#ifndef OCTAVE_CORE_WINDOW_HPP
#define OCTAVE_CORE_WINDOW_HPP

#include "core/Callback.hpp"
#include <string>

namespace Octave {

struct WindowOptions {
	int width;
	int height;
	bool is_fullscreen;
	bool is_borderless;
	std::string title;

	WindowOptions() noexcept;
	WindowOptions( std::string title ) noexcept;
	WindowOptions( int width, int height, std::string title,
				   bool is_fullscreen = false,
				   bool is_borderless = false ) noexcept;
};

class Window {
public:
	explicit Window( const WindowOptions& options ) {};
	virtual ~Window() noexcept = default;

	[[nodiscard]] virtual void* GetNativeHandle() const noexcept = 0;
	[[nodiscard]] virtual std::pair<int, int> GetSize() const noexcept = 0;
	[[nodiscard]] virtual bool IsOpen() const noexcept = 0;

	virtual Window& SetTitle( const std::string& title ) noexcept = 0;
	// Window& SetSyncInterval( int interval ) noexcept;

	// Actions
	void Close() const noexcept;
	void PollEvents() noexcept;
	// void SwapBuffers() noexcept;

	// Callbacks
	Callback<void, int, int> OnSizeChanged;
	Callback<void> OnClose;

	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
};

}  // namespace Octave

#endif  // OCTAVE_WINDOW_HPP
