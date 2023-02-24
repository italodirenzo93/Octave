#include "Sample.hpp"

using namespace std;
using namespace Octave;

void Sample::OnInitialize() {
	step_timer_ = make_unique<StepTimer>();

	GetWindow().AddCloseCallback( [this]() { Exit(); } );

	renderer_ = GetGraphicsSystem().CreateRenderer();
	const auto [default_width, default_height] = GetWindow().GetSize();
	renderer_->SetViewport( 0, 0, default_width, default_height );

	GetWindow().AddSizeChangedCallback( [this]( int width, int height ) {
		if ( width > 0 && height > 0 ) {
			renderer_->SetViewport( 0, 0, width, height );
			camera_.width_ = static_cast<float>( width );
			camera_.height_ = static_cast<float>( height );
		}
	} );
}

void Sample::OnUpdate() {
	step_timer_->Tick( [this] { OnStep(); } );
	OnRender();
}
