#include "Application.hpp"

#include "core/Log.hpp"
#include "input/glfw/InputSystemGLFW.hpp"

namespace Octave {

Application::Application() : Application( 0, nullptr ) {
}

Application::Application( int argc, char* argv[] ) : is_running_( true ) {
	Log::Init();

	input_ = MakeRef<Impl::InputSystemGLFW>();
	window_ = Window::Create( WindowOptions() );
    graphics_ = MakeRef<GraphicsDevice>( *window_ );

	// Set window close callback
	window_->AddCloseCallback( [this] { Exit(); } );
}

Application::~Application() noexcept {
	// Release all the layers
	while ( !layers_.IsEmpty() ) {
		PopLayer();
	}
}


void Application::Run() {
	OnInitialize();
	while ( is_running_ ) {
		window_->PollEvents();

		for ( const auto& layer : layers_ ) {
			layer->OnUpdate();
		}

		window_->SwapBuffers();
	}
	OnExit();
}

void Application::Exit() {
	is_running_ = false;
}

Application& Application::PushLayer( LayerStack::LayerPtr layer ) noexcept {
	layer->OnAttach();
	layers_.PushLayer( std::move( layer ) );
	return *this;
}

LayerStack::LayerPtr Application::PopLayer() noexcept {
	auto layer = layers_.PopLayer();
	layer->OnDetach();
	return layer;
}

}  // namespace Octave
