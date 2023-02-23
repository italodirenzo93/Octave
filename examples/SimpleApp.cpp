#include <Octave.hpp>
#include <EntryPoint.hpp>

#include <iostream>
#include <memory>

using namespace std;

class SimpleApp : public Octave::Application {
public:
	void Initialize() override {
		m_window = platform_->CreateWindow( Octave::WindowOptions() );
		m_window->AddCloseCallback( [this]() { Exit(); } );

		m_renderer = make_unique<Octave::Renderer>();
	}

	void Update() override {
		m_renderer->Clear( true, false, 0.1f, 0.6f, 0.5f );
		m_window->SwapBuffers();
	}

private:
	unique_ptr<Octave::Window> m_window;
	unique_ptr<Octave::Renderer> m_renderer;
};

unique_ptr<Octave::Application> Octave::CreateApplication( int argc, char* argv[] ) {
	return make_unique<SimpleApp>();
}
