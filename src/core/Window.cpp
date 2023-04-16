#include "pch/pch.hpp"
#include "Config.hpp"

#include "core/Window.hpp"

namespace Octave {

WindowOptions::WindowOptions() noexcept
	: WindowOptions( "Octave Application" ) {
}

WindowOptions::WindowOptions( std::string title ) noexcept : m_Title( title ) {
	m_Width = Config::Instance().GetFramebufferWidth();
	m_Height = Config::Instance().GetFramebufferHeight();
	m_IsFullscreen = Config::Instance().IsFullscreen();
	m_IsBorderless = Config::Instance().IsBorderless();
}

WindowOptions::WindowOptions( int width, int height, std::string title,
							  bool is_fullscreen, bool is_borderless ) noexcept
	: m_Width( width ),
	  m_Height( height ),
	  m_IsFullscreen( is_fullscreen ),
	  m_IsBorderless( is_borderless ),
	  m_Title( std::move( title ) ) {
}

}  // namespace Octave
