#ifndef OCTAVE_PLATFORM_HPP
#define OCTAVE_PLATFORM_HPP

#include <cstdint>

namespace Octave {

enum class PlatformName { Unknown, Windows, Mac, Linux };

class Platform {
public:
	static bool Init();
	static void Quit();

	static PlatformName GetName() noexcept;

	static double GetElapsedTime() noexcept;
	static uint64_t GetTimerFrequency() noexcept;
	static uint64_t GetTimerValue() noexcept;

	Platform() = delete;
	Platform( const Platform& ) = delete;
	Platform( Platform&& ) = delete;
	Platform& operator=( const Platform& ) = delete;
	Platform& operator=( Platform&& ) noexcept = delete;
};

}  // namespace Octave

#endif  // OCTAVE_PLATFORM_HPP
