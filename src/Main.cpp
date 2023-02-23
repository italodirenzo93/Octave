#include "Octave.hpp"
#include "samples/ModelViewerSample.hpp"

using namespace std;
using namespace Octave;

int main() {
	try {
		unique_ptr<Application> app( new Samples::ModelViewerSample() );
		app->Run();
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
