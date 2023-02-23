#include "Octave.hpp"

using namespace std;
using namespace Octave;

class SampleApplication : public Application {
protected:
	void Initialize() override {
		window_ = platform_->CreateWindow( WindowOptions() );
		window_->AddCloseCallback( [this]() { Exit(); } );

		renderer_ = make_unique<Renderer>();
		const auto [default_width, default_height] = window_->GetSize();
		renderer_->SetViewport( 0, 0, default_width, default_height );

		window_->AddSizeChangedCallback( [this]( int width, int height ) {
			renderer_->SetViewport( 0, 0, width, height );
		} );
	}

	void Update() override {
		renderer_->Clear( true, false, 0.0f, 1.0f, 0.0f, 1.0f );

		window_->SwapBuffers();
		window_->PollEvents();
	}

private:
	unique_ptr<Window> window_;
	unique_ptr<Renderer> renderer_;
};

int main() {
	try {
		unique_ptr<Application> app( new SampleApplication() );
		app->Run();
	} catch ( const std::exception& e ) {
		cerr << "Critical Error: " << e.what() << " ...exiting" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
