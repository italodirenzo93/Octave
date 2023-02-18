#include "Application.hpp"

#include "platform/glfw/PlatformGLFW.hpp"

namespace octave {

Application::Application() : is_running_( true ) {
	platform_ = std::make_unique<platform::glfw::PlatformGLFW>();
}

Application::~Application() noexcept {
	platform_.reset();
}

void Application::Run() {
	Initialize();
	while ( is_running_ ) {
		Update();
		Render();
	}
}

void Application::Exit() {
	is_running_ = false;
}

void Application::Initialize() {
}

void Application::Update() {
}

void Application::Render() {
}

}  // namespace octave
