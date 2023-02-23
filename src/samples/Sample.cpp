#include "Sample.hpp"

namespace octave::samples {

Sample::Sample( const std::shared_ptr<graphics::Window>& window ) noexcept
	: window_( window ), renderer_( *window ) {
	std::function<void()> close_callback = [this] { Exit(); };
	window_->AddCloseCallback( close_callback );
}

void Sample::Run() {
	OnLoad();

	while ( !should_quit_ ) {
		step_timer_.Tick( [&] { OnUpdate( step_timer_ ); } );
		OnRender();
		window_->PollEvents();
	}

	OnUnload();
}

void Sample::Exit() {
	should_quit_ = true;
}

}  // namespace octave::samples
