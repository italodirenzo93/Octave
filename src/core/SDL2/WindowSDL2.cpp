#include "pch/pch.hpp"
#include "WindowSDL2.hpp"

namespace Octave::Impl {

WindowSDL2::WindowSDL2( const WindowOptions& options ) : Window( options ) {
	int flags = 0;

	if ( options.is_borderless ) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	} else if ( options.is_fullscreen ) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

#ifdef OGT_RHI_OPENGL
	flags |= SDL_WINDOW_OPENGL;
#endif

	m_Window = SDL_CreateWindow( options.title.c_str(), SDL_WINDOWPOS_UNDEFINED,
								 SDL_WINDOWPOS_UNDEFINED, options.width,
								 options.height, flags );
}

}  // namespace Octave::Impl
