#ifndef OCTAVE_EXAMPLES_COMMON_SAMPLEAPPLICATION_HPP
#define OCTAVE_EXAMPLES_COMMON_SAMPLEAPPLICATION_HPP

#include <Octave.hpp>

namespace Octave::Samples {

class SampleApplication : public Application {
protected:
	void OnInitialize() override {
		// Exit on window close
		GetWindow().OnClose.Add( [this] { Exit(); } );

		// Rendering context
		context_ = GetGraphicsDevice().CreateContext();

		GetWindow().OnSizeChanged.Add( [this]( int width, int height ) {
			context_->SetViewport( 0, 0, width, height );
		} );
	}

	void OnUpdate() override {
		step_timer_.Tick( [this] { Update(); } );
		Draw();
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	Ref<GraphicsContext> context_;
	StepTimer step_timer_;
};

#define SAMPLE_MAIN(ClassName) \
	Ref<Octave::Application> Octave::CreateApplication( int argc, char* argv[] ) { \
		return MakeRef<ClassName>(); \
	}

}

#endif