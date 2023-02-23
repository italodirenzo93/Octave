#include "core/Platform.hpp"

#include <GLFW/glfw3.h>

namespace Octave {

PlatformName Platform::GetName() noexcept {
#if defined( OCTAVE_PLATFORM_WINDOWS )
	return PlatformName::Windows;
#elif defined( OCTAVE_PLATFORM_MACOS )
	return PlatformName::Mac;
#elif defined( OCTAVE_PLATFORM_LINUX )
	return PlatformName::Linux;
#else
#error "Unsupported platform"
#endif
}

double Platform::GetElapsedTime() noexcept {
	return glfwGetTime();
}

uint64_t Platform::GetTimerFrequency() noexcept {
	return glfwGetTimerFrequency();
}

uint64_t Platform::GetTimerValue() noexcept {
	return glfwGetTimerValue();
}

}  // namespace Octave
