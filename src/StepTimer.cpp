#include "StepTimer.hpp"

#include "CommonInclude.hpp"

namespace octave {

StepTimer::StepTimer() noexcept {
	last_frame_time_ = elapsed_time_ = delta_time_ = 0.0;
}

StepTimer::StepTimer( float elapsed ) noexcept {
	last_frame_time_ = elapsed;
	elapsed_time_ = elapsed;
	delta_time_ = 0.0;
}

void StepTimer::Tick() noexcept {
	const auto current_time = glfwGetTime();
	delta_time_ = current_time - last_frame_time_;
	last_frame_time_ = current_time;
}

}  // namespace octave
