#ifndef OCTAVE_CORE_WINDOW_HPP
#define OCTAVE_CORE_WINDOW_HPP

#include "core/Callback.hpp"
#include "graphics/GraphicsDevice.hpp"

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
	Window() = default;
	virtual ~Window() noexcept = default;

	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

	[[nodiscard]] virtual void* GetNativeWindowHandle() = 0;
	[[nodiscard]] virtual std::pair<int, int> GetSize() = 0;

	virtual void SetTitle( const std::string& title ) = 0;
	virtual void SetSyncInterval( int interval ) = 0;

	// Actions
	virtual void Close() = 0;
	virtual void PollEvents() = 0;

	// Callbacks
	Callback<void, int, int> OnSizeChanged;
	Callback<void> OnClose;
};

}  // namespace Octave

#endif  // OCTAVE_WINDOW_HPP
