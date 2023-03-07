#ifndef OCTAVE_ENTRYPOINT_HPP
#define OCTAVE_ENTRYPOINT_HPP

#include "Application.hpp"
#include "core/Log.hpp"

extern Octave::Ref<Octave::Application> Octave::CreateApplication( int argc, char* argv[] );

int main( int argc, char* argv[] ) {
	try {
		auto app = Octave::CreateApplication( argc, argv );
		app->Run();
	} catch ( const std::exception& e ) {
		Octave::Log::GetCoreLogger()->critical( "{}", e.what() );
		return 1;
	}

	return 0;
}

#endif
