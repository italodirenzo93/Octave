#include "Sample.hpp"

using namespace std;
using namespace Octave;

SampleLayer::SampleLayer( const Sample& app ) {
	step_timer_ = make_unique<StepTimer>();

	renderer_ = app.GetGraphicsDevice().CreateRenderer();

	app.GetWindow().AddSizeChangedCallback( [this]( int width, int height ) {
		renderer_->SetViewport( 0, 0, width, height );
	} );
}


void Sample::OnInitialize() {

}
