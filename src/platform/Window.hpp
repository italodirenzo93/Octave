#ifndef OCTAVE_WINDOW_H
#define OCTAVE_WINDOW_H

#include "pch.hpp"

namespace Octave {

struct WindowOptions {
	bool is_fullscreen;
	bool is_borderless;
	int width;
	int height;
	std::string title;
};

class Window {
public:
	virtual ~Window() noexcept = default;

	[[nodiscard]] virtual std::pair<int, int> GetSize() const noexcept = 0;
	[[nodiscard]] virtual bool IsOpen() const noexcept = 0;

	virtual Window& SetTitle( const std::string& title ) noexcept = 0;
	virtual Window& SetSyncInterval( int interval ) noexcept = 0;

	// Actions
	virtual void PollEvents() const noexcept = 0;
	virtual void Close() const noexcept = 0;
	virtual void SwapBuffers() noexcept = 0;

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

protected:
	explicit Window( const WindowOptions& options ) {};

	std::vector<OnSizeChangedCallback> cb_window_size_;
	std::vector<OnCloseCallback> cb_close_;
};

}  // namespace octave::platform

#endif  // OCTAVE_WINDOW_H
