#ifndef OCTAVE_CORE_SDL2_WINDOW_HPP
#define OCTAVE_CORE_SDL2_WINDOW_HPP

#include "core/Window.hpp"

#include <SDL.h>

namespace Octave::Impl {

class WindowSDL2 final : public Window {
public:
	explicit WindowSDL2( const WindowOptions& options );
	~WindowSDL2() noexcept override;


private:
	SDL_Window* m_Window;
};

}

#endif
