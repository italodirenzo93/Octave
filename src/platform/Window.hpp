#ifndef OCTAVE_WINDOW_HPP
#define OCTAVE_WINDOW_HPP

#include <functional>
#include <string>
#include <vector>

namespace Octave {

struct WindowOptions {
	bool is_fullscreen;
	bool is_borderless;
	int width;
	int height;
	std::string title;

	WindowOptions() noexcept;
	WindowOptions( std::string title ) noexcept;
	WindowOptions( int width, int height, std::string title,
				   bool is_fullscreen = false,
				   bool is_borderless = false ) noexcept;
};

class Window {
public:
	virtual ~Window() noexcept = default;

	[[nodiscard]] virtual std::pair<int, int> GetSize() const noexcept = 0;
	[[nodiscard]] virtual bool IsOpen() const noexcept = 0;

	virtual Window& SetTitle( const std::string& title ) noexcept = 0;
	virtual Window& SetSyncInterval( int interval ) noexcept = 0;

	// Actions
	virtual void Close() const noexcept = 0;
	virtual void SwapBuffers() noexcept = 0;
	virtual void PollEvents() noexcept = 0;

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

	static std::unique_ptr<Window> Create( const WindowOptions& options );

protected:
	Window() = default;

	std::vector<OnSizeChangedCallback> cb_window_size_;
	std::vector<OnCloseCallback> cb_close_;

public:
	Window( const Window& ) = delete;
	Window( Window&& ) = delete;

	Window& operator=( const Window& ) = delete;
	Window& operator=( Window&& ) = delete;
};

}  // namespace Octave

#endif  // OCTAVE_WINDOW_HPP
