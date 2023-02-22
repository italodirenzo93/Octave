#ifndef OCTAVE_ENTRYPOINT_HPP
#define OCTAVE_ENTRYPOINT_HPP

#include "Application.hpp"

extern std::unique_ptr<Octave::Application> Octave::CreateApplication( int argc, char* argv[] );

int main( int argc, char* argv[] ) {
	try {
		auto app = Octave::CreateApplication( argc, argv );
		app->Run();
	} catch ( const std::exception& e ) {
		std::cerr << "Critical Error: " << e.what() << " ...exiting" << std::endl;
		return 1;
	}

	return 0;
}

#endif
