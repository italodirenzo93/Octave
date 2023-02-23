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

double PlatformGLFW::GetElapsedTime() const {
	return glfwGetTime();
}

uint64_t PlatformGLFW::GetPerformanceCounter() const {
	return glfwGetTimerValue();
}

uint64_t PlatformGLFW::GetPerformanceFrequency() const {
	return glfwGetTimerFrequency();
}

PlatformName PlatformGLFW::GetName() const {
#if defined(OCTAVE_PLATFORM_WINDOWS)
	return PlatformName::Windows;
#elif defined(OCTAVE_PLATFORM_MACOS)
	return PlatformName::Mac;
#elif defined(OCTAVE_PLATFORM_LINUX)
	return PlatformName::Linux;
#else
#error Unsupported platform
#endif
}

void PlatformGLFW::PollEvents() noexcept {
	glfwPollEvents();
}

std::unique_ptr<Window> PlatformGLFW::CreateWindow(
	const WindowOptions& options ) {
	return std::make_unique<WindowGLFW>( options );
}

}  // namespace Octave::Impl
