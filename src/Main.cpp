#include "Octave.hpp"
#include "samples/ModelViewerSample.hpp"
#include "samples/Sample.hpp"

using namespace std;
using namespace octave;
using namespace octave::graphics;
using namespace octave::samples;

int main( int argc, char* argv[] ) {
	// Initialize engine
	octave::Up();

	std::unique_ptr<Sample> sample;

	try {
		shared_ptr<Window> window( new Window() );

		sample = make_unique<ModelViewerSample>( window );

		sample->Run();

	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;

		octave::Down();
		return EXIT_FAILURE;
	}

	octave::Down();
	return EXIT_SUCCESS;
}
