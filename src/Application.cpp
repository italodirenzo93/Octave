#include "Application.hpp"

#include "core/Log.hpp"
#include "input/Null/InputSystemNull.hpp"
#include "graphics/OpenGL/OpenGLGraphicsDevice.hpp"

#ifdef OGT_PLATFORM_WINDOWS
#undef CreateWindow
#endif

namespace Octave {

ApplicationOptions::ApplicationOptions( int argc, char* argv[] )
	: m_RenderApi( RenderApi::Default ), m_Args( argv + 1, argv + argc ) {
}

Application::Application( const ApplicationOptions& options )
	: m_IsRunning( true ) {
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
	while ( !m_Layers.IsEmpty() ) {
		PopLayer();
	}

	Platform::DestroyWindow( std::move( m_Window ) );
	Platform::Quit();
}


void Application::Run() {
	OnInitialize();
	while ( m_IsRunning ) {
		m_Window->PollEvents();

		OnUpdate();

		for ( const auto& layer : m_Layers ) {
			layer->OnUpdate();
		}

		m_Window->SwapBuffers();
	}
	OnExit();
}

void Application::Exit() {
	m_IsRunning = false;
}

Application& Application::PushLayer( LayerStack::LayerPtr layer ) noexcept {
	layer->OnAttach();
	m_Layers.PushLayer( std::move( layer ) );
	return *this;
}

LayerStack::LayerPtr Application::PopLayer() noexcept {
	auto layer = m_Layers.PopLayer();
	layer->OnDetach();
	return layer;
}

}  // namespace Octave
