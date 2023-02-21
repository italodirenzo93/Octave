#include "Octave.hpp"
#include "samples/ModelViewerSample.hpp"

using namespace std;
using namespace Octave;

int main( int argc, char* argv[] ) {
	try {
		string file_name = argc > 1 ? argv[1] : "";
		unique_ptr<Application> app( new Samples::ModelViewerSample( file_name ) );
		app->Run();
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
