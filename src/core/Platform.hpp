#ifndef OCTAVE_PLATFORM_HPP
#define OCTAVE_PLATFORM_HPP

#include "Window.hpp"
#include "graphics/GraphicsDevice.hpp"
#include "pch/pch.hpp"

#ifdef OGT_PLATFORM_WINDOWS
#undef CreateWindow
#endif

namespace Octave {

enum class PlatformName { Unknown, Windows, Mac, Linux };

class Platform {
public:
	static bool Init();
	static void Quit();

	static PlatformName GetName() noexcept;

	static uint64_t GetElapsedTicks() noexcept;
	static uint64_t GetCounterFrequency() noexcept;
	static uint64_t GetCounterValue() noexcept;

	[[nodiscard]] static std::unique_ptr<Window> CreateWindow(
		const WindowOptions& options );
	static void DestroyWindow( std::unique_ptr<Window> window );

	Platform() = delete;
	Platform( const Platform& ) = delete;
	Platform( Platform&& ) = delete;
	Platform& operator=( const Platform& ) = delete;
	Platform& operator=( Platform&& ) noexcept = delete;
};

}  // namespace Octave

#endif  // OCTAVE_PLATFORM_HPP
