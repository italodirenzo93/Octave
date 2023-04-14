#include "pch/pch.hpp"
#include "WindowSDL.hpp"

#include "graphics/OpenGL/OpenGLGraphicsDevice.hpp"

namespace Octave {

WindowSDL::WindowSDL( SDL_Window* window ) : m_Window( window ) {
	m_Device = std::make_unique<OpenGLGraphicsDevice>( *this );
}

std::pair<int, int> WindowSDL::GetSize() const {
	int width, height;
	SDL_GetWindowSize( m_Window, &width, &height );
	return { width, height };
}

void WindowSDL::SetTitle( const std::string& title ) {
	SDL_SetWindowTitle( m_Window, title.c_str() );
}

void WindowSDL::PollEvents() {
	SDL_Event evt;
	while ( SDL_PollEvent( &evt ) != 0) {
		if ( evt.type == SDL_QUIT ) {
			OnClose.InvokeAll();
		}
	}
}

void WindowSDL::SwapBuffers() {
	m_Device->SwapBuffers();
}

}
