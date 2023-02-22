#include <Octave.hpp>

#include <iostream>
#include <memory>

using namespace std;
using namespace Octave;

class SimpleApp : public Application {
public:
	void Initialize() override {
		m_window = platform_->CreateWindow(WindowOptions());
		m_window->AddCloseCallback( [this]() { Exit(); } );

		m_renderer = make_unique<Renderer>();
	}

	void Update() override {
		m_renderer->Clear(true, false, 0.1f, 0.6f, 0.2f);
		m_window->SwapBuffers();
	}

private:
	unique_ptr<Window> m_window;
	unique_ptr<Renderer> m_renderer;
};

int main() {
	try {
		auto app = make_unique<SimpleApp>();
		app->Run();
	} catch (const exception& e) {
		cout << "Exception: " << e.what() << endl;
		return 1;
	}
	return 0;
}
