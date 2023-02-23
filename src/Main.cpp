#include "Octave.hpp"

using namespace std;
using namespace octave;
using namespace octave::platform;

class SampleApplication : public Application {
protected:
	void Initialize() override {
		WindowOptions options = {};
		options.width = 800;
		options.height = 600;
		options.title = "Octave Game Toolkit";

		window_ = platform_->CreateWindow( options );
		window_->AddCloseCallback( [this]() { Exit(); } );
	}

	void Render() override { window_->PollEvents(); }

private:
	unique_ptr<Window> window_;
};

int main( int argc, char* argv[] ) {
	unique_ptr<Application> app;

	try {
		app = make_unique<SampleApplication>();
		app->Run();
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
