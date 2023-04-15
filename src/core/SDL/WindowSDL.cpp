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
		switch ( evt.type ) {
		case SDL_QUIT:
			OnClose.InvokeAll();
			break;
		case SDL_WINDOWEVENT: {
			switch ( evt.window.event ) {
			case SDL_WINDOWEVENT_RESIZED:
				OnSizeChanged.InvokeAll( evt.window.data1, evt.window.data2 );
				break;
			}
		}
		break;
		}
	}
}

void WindowSDL::SwapBuffers() {
	m_Device->SwapBuffers();
}

}
