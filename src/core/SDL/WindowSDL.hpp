#ifndef OCTAVE_CORE_SDL_WINDOWSDL_HPP
#define OCTAVE_CORE_SDL_WINDOWSDL_HPP

#include "core/Window.hpp"

#include <SDL.h>

namespace Octave {

class WindowSDL final : public Window {
public:
	explicit WindowSDL( SDL_Window* window );

	[[nodiscard]] GraphicsDevice& GetGraphicsDevice() const override { return *m_Device; }
	[[nodiscard]] void* GetNativeWindowHandle() const override { return m_Window; }
	[[nodiscard]] std::pair<int, int> GetSize() const override;

	void SetTitle( const std::string& title ) override;

	void PollEvents() override;
	void SwapBuffers() override;

private:
	SDL_Window* m_Window;
	std::unique_ptr<GraphicsDevice> m_Device;
};

}

#endif
