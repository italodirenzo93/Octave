#include "Application.hpp"

#include "platform/glfw/PlatformGLFW.hpp"

namespace Octave {

Application::Application() : Application( 0, nullptr ) {}

Application::Application( int argc, char* argv[] ) : is_running_( true ) {
	platform_ = std::make_unique<Impl::PlatformGLFW>();
}

Application::~Application() noexcept {
	platform_.reset();
}

void Application::Run() {
	Initialize();
	while ( is_running_ ) {
		Update();
	}
}

void Application::Exit() {
	is_running_ = false;
}

void Application::Initialize() {
}

void Application::Update() {
}

}  // namespace octave
