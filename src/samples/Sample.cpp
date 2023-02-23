#include "Sample.hpp"

using namespace std;

namespace Octave::Samples {

void Sample::Initialize() {
	step_timer_ = make_unique<StepTimer>( *platform_ );

	window_ = platform_->CreateWindow( WindowOptions() );
	window_->AddCloseCallback( [this]() { Exit(); } );

	renderer_ = make_unique<Renderer>();
	const auto [default_width, default_height] = window_->GetSize();
	renderer_->SetViewport( 0, 0, default_width, default_height );

	window_->AddSizeChangedCallback( [this]( int width, int height ) {
		if ( width > 0 && height > 0 ) {
			renderer_->SetViewport( 0, 0, width, height );
			camera_.width_ = static_cast<float>( width );
			camera_.height_ = static_cast<float>( height );
		}
	} );
}

void Sample::Update() {
	step_timer_->Tick( [this] { OnUpdate(); } );
	OnRender();

	window_->SwapBuffers();
}


}  // namespace Octave::Samples
