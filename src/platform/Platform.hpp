#ifndef OCTAVE_PLATFORM_HPP
#define OCTAVE_PLATFORM_HPP

#if defined( _WIN32 )
#define OCTAVE_PLATFORM_WINDOWS
#elif defined( __APPLE__ )
#define OCTAVE_PLATFORM_MACOS
#elif defined( __linux__ )
#define OCTAVE_PLATFORM_LINUX
#else
#error "Unknown platform"
#endif

#include "Window.hpp"

namespace Octave {

enum class PlatformName { Windows, Mac, Linux };

class Platform {
public:
	virtual ~Platform() = default;

	[[nodiscard]] virtual PlatformName GetName() const = 0;

	[[nodiscard]] virtual double GetElapsedTime() const = 0;
	[[nodiscard]] virtual uint64_t GetPerformanceCounter() const = 0;
	[[nodiscard]] virtual uint64_t GetPerformanceFrequency() const = 0;

	virtual void PollEvents() noexcept = 0;

	virtual std::unique_ptr<Window> CreateWindow(
		const WindowOptions& options ) = 0;
};

}  // namespace Octave

#endif  // OCTAVE_PLATFORM_HPP
