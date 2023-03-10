#include "Sample.hpp"

using namespace std;
using namespace Octave;

namespace Octave::Samples {

SampleLayer::SampleLayer( const Sample& app ) {
	step_timer_ = MakeRef<StepTimer>();

	context_ = app.GetGraphicsDevice().CreateContext();

	app.GetWindow().AddSizeChangedCallback( [this]( int width, int height ) {
		context_->SetViewport( 0, 0, width, height );
	} );
}


void Sample::OnInitialize() {
}

}  // namespace Octave::Samples
