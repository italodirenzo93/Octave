#include "Octave.hpp"

#include <fstream>

#include "CommonInclude.hpp"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

using namespace std;

namespace octave {

static bool b_is_initialized = false;

static ALCdevice* g_device = nullptr;
static ALCcontext* g_context = nullptr;

bool Up() {
	if ( b_is_initialized ) return false;

	// Initialize GLFW library
	if ( !glfwInit() ) {
		cout << "Failed to initialize GLFW" << endl;
		return false;
	}

	// Load game controller mappings
	{
		ifstream ifs( "./resources/config/gamecontrollerdb.txt" );
		if ( ifs.is_open() ) {
			cout << "gamecontrollerdb.txt found. Updating game controller DB..."
				 << endl;
			string mappings( ( istreambuf_iterator<char>( ifs ) ),
							 istreambuf_iterator<char>() );
			if ( !glfwUpdateGamepadMappings( mappings.c_str() ) ) {
				cerr << "Unable to update game controller DB" << endl;
			}
		}
	}

	// Initialize OpenAL Soft
	g_device = alcOpenDevice( nullptr );
	if ( g_device ) {
		g_context = alcCreateContext( g_device, nullptr );
		alcMakeContextCurrent( g_context );

		alGetError();  // clear error state
	} else {
		cerr << "No usable audio device" << endl;
	}

	b_is_initialized = true;
	return true;
}

void Down() {
	if ( !b_is_initialized ) return;

	if ( g_context ) {
		alcDestroyContext( g_context );
		g_context = nullptr;
	}

	if ( g_device ) {
		alcCloseDevice( g_device );
		g_device = nullptr;
	}

	glfwTerminate();

	b_is_initialized = false;
}

}  // namespace octave
