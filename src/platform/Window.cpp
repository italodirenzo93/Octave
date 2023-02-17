#include "Window.hpp"

#include "Config.hpp"

namespace octave::platform {

Window::Window() : Window( "Octave" ) {
}

Window::Window( const std::string& title )
	: Window( Config::Instance().GetFramebufferWidth(),
			  Config::Instance().GetFramebufferHeight(), title ) {
}

Window::Window( int width, int height, const std::string& title ) {
	// to be implemented per platform
}

}  // namespace octave::platform
