#include "Octave.hpp"

using namespace std;
using namespace Octave;

class SampleApplication : public Application {
public:
	SampleApplication( int argc, char* argv[] ) : Application( argc, argv ) {}

protected:
	void Initialize() override {
		WindowOptions options = {};
		options.width = 800;
		options.height = 600;
		options.title = "Octave Game Toolkit";

		window_ = platform_->CreateWindow( options );
		window_->AddCloseCallback( [this]() { Exit(); } );
	}

	void Update() override {
		window_->SwapBuffers();
		window_->PollEvents();
	}

private:
	unique_ptr<Window> window_;
};

int main( int argc, char* argv[] ) {
	try {
		unique_ptr<Application> app( new SampleApplication( argc, argv ) );
		app->Run();
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
