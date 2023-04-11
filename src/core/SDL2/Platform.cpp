#include "pch/pch.hpp"
#include "core/Platform.hpp"
#include "core/Log.hpp"

#include <SDL.h>

namespace Octave {

static bool g_initialized = false;

bool Platform::Init() {
	if ( g_initialized ) {
		Log::GetCoreLogger()->trace( "Platform module already initialized" );
		return;
	}

	const int result = SDL_Init( SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER );

	if ( result != 0 ) {
		Log::GetCoreLogger()->critical( "SDL_Init returned {0}. Platform initialization failed.", result );
		return false;
	}

	g_initialized = true;
	return true;
}

void Platform::Quit() {
	SDL_Quit();
	g_initialized = false;
}

PlatformName Platform::GetName() noexcept {
	const auto name = SDL_GetPlatform();
	if ( name == "Windows" ) {
		return PlatformName::Windows;
	} else if ( name == "Mac OS X" ) {
		return PlatformName::Mac;
	} else if ( name == "Linux" ) {
		return PlatformName::Linux;
	} else {
		return PlatformName::Unknown;
	}
}

double Platform::GetElapsedTime() noexcept {
	return SDL_GetTicks64() / 1000.0;
}

uint64_t Platform::GetTimerFrequency() noexcept {
	return SDL_GetPerformanceFrequency();
}

uint64_t Platform::GetTimerValue() noexcept {
	return SDL_GetPerformanceCounter();
}

}
