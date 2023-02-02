#ifndef OCTAVE_STEPTIMER_HPP
#define OCTAVE_STEPTIMER_HPP

namespace octave {

class StepTimer {
public:
	StepTimer() noexcept;
	explicit StepTimer( float elapsed ) noexcept;
	~StepTimer() = default;

	[[nodiscard]] float GetDeltaTime() const noexcept {
		return static_cast<float>( delta_time_ );
	}

	void Tick() noexcept;

private:
	double last_frame_time_;
	double elapsed_time_;
	double delta_time_;
};

}  // namespace octave

#endif  // OCTAVE_STEPTIMER_HPP
