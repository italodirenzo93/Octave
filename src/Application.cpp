#include "Application.hpp"

#include "core/Log.hpp"
#include "input/Null/InputSystemNull.hpp"
#include "graphics/OpenGL/OpenGLGraphicsDevice.hpp"

#ifdef OGT_PLATFORM_WINDOWS
#undef CreateWindow
#endif

namespace Octave {

Application::Application( const ApplicationOptions& options ) : is_running_( true ) {
	Log::Init();
	if ( !Platform::Init() ) {
		throw Exception( "Unable to initialize platform" );
	}

	m_Input = std::make_unique<InputSystemNull>();

	WindowOptions windowOpts;
	windowOpts.m_RenderApi = RenderApi::OpenGL;
	m_Window = Platform::CreateWindow( windowOpts );
}

Application::~Application() noexcept {
	// Release all the layers
	while ( !layers_.IsEmpty() ) {
		PopLayer();
	}

	Platform::DestroyWindow( std::move( m_Window ) );
	Platform::Quit();
}


void Application::Run() {
	OnInitialize();
	while ( is_running_ ) {
		m_Window->PollEvents();

		OnUpdate();

		for ( const auto& layer : layers_ ) {
			layer->OnUpdate();
		}

		m_Window->SwapBuffers();
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
