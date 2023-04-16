#include "Config.hpp"
#include "core/Platform.hpp"
#include "core/Log.hpp"

#include "SDL/WindowSDL.hpp"

#ifdef OGT_PLATFORM_WINDOWS
#undef CreateWindow
#endif

namespace Octave {

static bool g_initialized = false;

bool Platform::Init() {
	if ( g_initialized ) return true;

	Log::GetCoreLogger()->trace( "Initializing platform module" );

	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) != 0 ) {
		Log::GetCoreLogger()->error( "Failed to initialize SDL: {}",
									 SDL_GetError() );
		return false;
	}

	g_initialized = true;
	return true;
}

void Platform::Quit() {
	if ( !g_initialized ) return;

	Log::GetCoreLogger()->trace( "Shutting down platform module" );
	SDL_Quit();
	g_initialized = false;
}

PlatformName Platform::GetName() noexcept {
	const char* platform = SDL_GetPlatform();
	if ( platform == "Windows" ) {
		return PlatformName::Windows;
	}
	if ( platform == "Mac OS X" ) {
		return PlatformName::Mac;
	}
	if ( platform == "Linux" ) {
		return PlatformName::Linux;
	}

	return PlatformName::Unknown;
}

uint64_t Platform::GetElapsedTicks() noexcept {
	return SDL_GetTicks64();
}

uint64_t Platform::GetCounterFrequency() noexcept {
	return SDL_GetPerformanceFrequency();
}

uint64_t Platform::GetCounterValue() noexcept {
	return SDL_GetPerformanceCounter();
}

std::unique_ptr<Window> Platform::CreateWindow( const WindowOptions& options ) {
	int flags = SDL_WINDOW_RESIZABLE;

	if ( options.m_IsBorderless ) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	if ( options.m_IsFullscreen ) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if ( options.m_RenderApi == RenderApi::OpenGL ) {
		flags |= SDL_WINDOW_OPENGL;
	}

	const auto window = SDL_CreateWindow(
		options.m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		options.m_Width, options.m_Height, flags );

	if ( !window ) {
		Log::GetCoreLogger()->error( "Unable to create SDL window: {}",
									 SDL_GetError() );
		return nullptr;
	}

	return std::make_unique<WindowSDL>( window );
}

void Platform::DestroyWindow( std::unique_ptr<Window> window ) {
	assert( window != nullptr );
	SDL_DestroyWindow(
		static_cast<SDL_Window*>( window->GetNativeWindowHandle() ) );
}

}  // namespace Octave
