#include "Application.hpp"

#include "core/Log.hpp"
#include "input/glfw/InputSystemGLFW.hpp"

namespace Octave {

Application::Application() : Application( 0, nullptr ) {
}

Application::Application( int argc, char* argv[] ) : is_running_( true ) {
	Log::Init();

	input_ = std::make_unique<Impl::InputSystemGLFW>();
	window_ = Window::Create( WindowOptions() );
}

Application::~Application() noexcept {
}

void Application::Run() {
	OnInitialize();
	while ( is_running_ ) {
		window_->PollEvents();
		OnUpdate();
		window_->SwapBuffers();
	}
	OnExit();
}

void Application::Exit() {
	is_running_ = false;
}

void Application::OnInitialize() {
}

void Application::OnUpdate() {
}

void Application::OnExit() noexcept {
}

}  // namespace Octave
