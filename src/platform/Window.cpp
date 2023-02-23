#include "Window.hpp"

#include "Config.hpp"

namespace Octave {

WindowOptions::WindowOptions() noexcept {
	width = Config::Instance().GetFramebufferWidth();
	height = Config::Instance().GetFramebufferHeight();
	title = "Octave Game Toolkit";
	is_fullscreen = Config::Instance().IsFullscreen();
	is_borderless = Config::Instance().IsBorderless();
}

WindowOptions::WindowOptions( int width, int height, std::string title,
							  bool is_fullscreen, bool is_borderless ) noexcept
	: width( width ),
	  height( height ),
	  title( std::move( title ) ),
	  is_fullscreen( is_fullscreen ),
	  is_borderless( is_borderless ) {
}

}  // namespace Octave
