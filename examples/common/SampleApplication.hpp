#ifndef OCTAVE_EXAMPLES_COMMON_SAMPLEAPPLICATION_HPP
#define OCTAVE_EXAMPLES_COMMON_SAMPLEAPPLICATION_HPP

#include <Octave.hpp>

namespace Octave::Samples {

class SampleApplication : public Application {
public:
	SampleApplication( const ApplicationOptions& opts ) : Application( opts ) {}

protected:
	void OnInitialize() override {
		// Exit on window close
		GetWindow().OnClose.Add( [this] { Exit(); } );

		// Rendering context
		m_Context = GetGraphicsDevice().CreateContext();

		GetWindow().OnSizeChanged.Add( [this]( int width, int height ) {
			m_Context->SetViewport( 0, 0, width, height );
		} );
	}

	void OnUpdate() override {
		m_StepTimer.Tick( [this] { Update(); } );
		Draw();
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	std::unique_ptr<GraphicsContext> m_Context;
	StepTimer m_StepTimer;

public:
	virtual ~SampleApplication() noexcept {
		GetGraphicsDevice().DestroyContext( std::move( m_Context ));
	}
};

#define SAMPLE_MAIN(ClassName) \
	std::unique_ptr<Octave::Application> Octave::CreateApplication( int argc, char* argv[] ) {\
		ApplicationOptions opts( argc, argv );\
		return std::make_unique<ClassName>( opts );\
	}

}

#endif
