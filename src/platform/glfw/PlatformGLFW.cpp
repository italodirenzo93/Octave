#include "PlatformGLFW.hpp"

#include <GLFW/glfw3.h>

#include "GLFWError.hpp"
#include "WindowGLFW.hpp"

namespace Octave::Impl {

PlatformGLFW::PlatformGLFW() {
	if ( !glfwInit() ) {
		throw GLFWError();
	}
}

PlatformGLFW::~PlatformGLFW() noexcept {
	glfwTerminate();
}

double PlatformGLFW::GetElapsedTime() {
	return glfwGetTime();
}

uint64_t PlatformGLFW::GetPerformanceCounter() {
	return glfwGetTimerValue();
}

uint64_t PlatformGLFW::GetPerformanceFrequency() {
	return glfwGetTimerFrequency();
}

PlatformName PlatformGLFW::GetName() const {
#if defined( OCTAVE_PLATFORM_WINDOWS )
	return PlatformName::Windows;
#elif defined( OCTAVE_PLATFORM_MACOS )
	return PlatformName::MacOs;
#elif defined( OCTAVE_PLATFORM_LINUX )
	return PlatformName::Linux;
#else
#error "Unknown platform"
#endif
}

std::unique_ptr<Window> PlatformGLFW::CreateWindow(
	const WindowOptions& options ) {
	return std::make_unique<WindowGLFW>( options );
}

}  // namespace octave::platform::glfw
