// clang-format off
#include <Octave.hpp>
#include <EntryPoint.hpp>
// clang-format on
#include <iostream>
#include <memory>

using namespace std;

class SimpleApp : public Octave::Application {
public:
	void OnInitialize() override {
		window_->AddCloseCallback( [this]() { Exit(); } );

		m_renderer = make_unique<Octave::Renderer>();

		Octave::Log::Info( "Hello World!" );
	}

	void OnUpdate() override {
		m_renderer->Clear( true, false, 0.1f, 0.6f, 0.5f );
	}

private:
	unique_ptr<Octave::Renderer> m_renderer;
};

unique_ptr<Octave::Application> Octave::CreateApplication( int argc,
														   char* argv[] ) {
	return make_unique<SimpleApp>();
}
