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

#include <cstdint>
#include <string>

namespace octave::platform {

enum PlatformName { Windows, MacOs, Linux };

class Platform {
public:
	virtual ~Platform() = default;
	[[nodiscard]] virtual PlatformName GetName() const = 0;
	virtual double GetElapsedTime() = 0;
	virtual uint64_t GetPerformanceCounter() = 0;
	virtual uint64_t GetPerformanceFrequency() = 0;
};

}  // namespace octave::platform

#endif  // OCTAVE_PLATFORM_HPP
