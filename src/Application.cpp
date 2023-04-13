#include "Application.hpp"

#include "core/Log.hpp"
#include "input/glfw/InputSystemGLFW.hpp"
#include "graphics/OpenGL/OpenGLGraphicsDevice.hpp"

namespace Octave {

Application::Application() : Application( 0, nullptr ) {
}

Application::Application( int argc, char* argv[] ) : is_running_( true ) {
	Log::Init();
	if ( !Platform::Init() ) {
		throw Exception( "Unable to initialize platform" );
	}

	input_ = std::make_unique<Impl::InputSystemGLFW>();
	window_ = Platform::CreateWindow( WindowOptions() );
    graphics_ = std::make_unique<OpenGLGraphicsDevice>( *window_ );
}

Application::~Application() noexcept {
	// Release all the layers
	while ( !layers_.IsEmpty() ) {
		PopLayer();
	}

	Platform::DestroyWindow( std::move( window_ ) );
	Platform::Quit();
}


void Application::Run() {
	OnInitialize();
	while ( is_running_ ) {
		window_->PollEvents();

		OnUpdate();

		for ( const auto& layer : layers_ ) {
			layer->OnUpdate();
		}

		graphics_->SwapBuffers();
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
