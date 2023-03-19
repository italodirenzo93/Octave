#include "pch/pch.hpp"
#include "Config.hpp"

#include "core/Window.hpp"

namespace Octave {

WindowOptions::WindowOptions() noexcept
	: WindowOptions( "Octave Application" ) {
}

WindowOptions::WindowOptions( std::string title ) noexcept : title( title ) {
	width = Config::Instance().GetFramebufferWidth();
	height = Config::Instance().GetFramebufferHeight();
	is_fullscreen = Config::Instance().IsFullscreen();
	is_borderless = Config::Instance().IsBorderless();
}

WindowOptions::WindowOptions( int width, int height, std::string title,
							  bool is_fullscreen, bool is_borderless ) noexcept
	: width( width ),
	  height( height ),
	  is_fullscreen( is_fullscreen ),
	  is_borderless( is_borderless ),
	  title( std::move( title ) ) {
}

}  // namespace Octave
