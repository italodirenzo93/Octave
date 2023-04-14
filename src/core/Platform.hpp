#ifndef OCTAVE_PLATFORM_HPP
#define OCTAVE_PLATFORM_HPP

#include "pch/pch.hpp"
#include "Window.hpp"
#include "graphics/GraphicsDevice.hpp"

#ifdef OGT_PLATFORM_WINDOWS
#undef CreateWindow
#endif

namespace Octave {

enum class PlatformName { Windows, Mac, Linux };

class Platform {
public:
	static bool Init();
	static void Quit();

	static PlatformName GetName() noexcept;

	static double GetElapsedTime() noexcept;
	static uint64_t GetTimerFrequency() noexcept;
	static uint64_t GetTimerValue() noexcept;

	[[nodiscard]] static std::unique_ptr<Window> CreateWindow( const WindowOptions& options );
	static void DestroyWindow( std::unique_ptr<Window> window );

	Platform() = delete;
	Platform( const Platform& ) = delete;
	Platform( Platform&& ) = delete;
	Platform& operator=( const Platform& ) = delete;
	Platform& operator=( Platform&& ) noexcept = delete;
};

}  // namespace Octave

#endif  // OCTAVE_PLATFORM_HPP
