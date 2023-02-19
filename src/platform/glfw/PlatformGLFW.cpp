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
	const int glfw_platform = glfwGetPlatform();

	assert( glfwPlatformSupported( glfw_platform ) == GLFW_TRUE );
	assert( glfw_platform != GLFW_NOT_INITIALIZED );
	assert( glfw_platform != GLFW_PLATFORM_NULL );

	switch ( glfw_platform ) {
		case GLFW_PLATFORM_COCOA:
			return PlatformName::Mac;
		case GLFW_PLATFORM_WAYLAND:
		case GLFW_PLATFORM_X11:
			return PlatformName::Linux;
		case GLFW_PLATFORM_WIN32:
			return PlatformName::Windows;
		default:
			assert( false );
	}
}

std::unique_ptr<Window> PlatformGLFW::CreateWindow(
	const WindowOptions& options ) {
	return std::make_unique<WindowGLFW>( options );
}

}  // namespace Octave::Impl
