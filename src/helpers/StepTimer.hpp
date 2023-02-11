#ifndef OCTAVE_STEPTIMER_HPP
#define OCTAVE_STEPTIMER_HPP

namespace octave::helpers {

// Helper class for animation and simulation timing.
class StepTimer {
public:
	StepTimer() noexcept( false )
		: m_elapsedTicks( 0 ),
		  m_totalTicks( 0 ),
		  m_leftOverTicks( 0 ),
		  m_frameCount( 0 ),
		  m_framesPerSecond( 0 ),
		  m_framesThisSecond( 0 ),
		  m_qpcSecondCounter( 0 ),
		  m_isFixedTimeStep( false ),
		  m_targetElapsedTicks( TicksPerSecond / 60 ) {
		m_qpcFrequency = glfwGetTimerFrequency();
		m_qpcLastTime = glfwGetTimerValue();

		// Initialize max delta to 1/10 of a second.
		m_qpcMaxDelta = static_cast<uint64_t>( m_qpcFrequency / 10 );
	}

	// Get elapsed time since the previous Update call.
	[[nodiscard]] uint64_t GetElapsedTicks() const noexcept {
		return m_elapsedTicks;
	}
	[[nodiscard]] double GetElapsedSeconds() const noexcept {
		return TicksToSeconds( m_elapsedTicks );
	}

	// Get total time since the start of the program.
	[[nodiscard]] uint64_t GetTotalTicks() const noexcept {
		return m_totalTicks;
	}
	[[nodiscard]] double GetTotalSeconds() const noexcept {
		return TicksToSeconds( m_totalTicks );
	}

	// Get total number of updates since start of the program.
	[[nodiscard]] uint32_t GetFrameCount() const noexcept {
		return m_frameCount;
	}

	// Get the current framerate.
	[[nodiscard]] uint32_t GetFramesPerSecond() const noexcept {
		return m_framesPerSecond;
	}

	// Set whether to use fixed or variable timestep mode.
	void SetFixedTimeStep( bool isFixedTimestep ) noexcept {
		m_isFixedTimeStep = isFixedTimestep;
	}

	// Set how often to call Update when in fixed timestep mode.
	void SetTargetElapsedTicks( uint64_t targetElapsed ) noexcept {
		m_targetElapsedTicks = targetElapsed;
	}
	void SetTargetElapsedSeconds( double targetElapsed ) noexcept {
		m_targetElapsedTicks = SecondsToTicks( targetElapsed );
	}

	// Integer format represents time using 10,000,000 ticks per second.
	static constexpr uint64_t TicksPerSecond = 10000000;

	static constexpr double TicksToSeconds( uint64_t ticks ) noexcept {
		return static_cast<double>( ticks ) / TicksPerSecond;
	}
	static constexpr uint64_t SecondsToTicks( double seconds ) noexcept {
		return static_cast<uint64_t>( seconds * TicksPerSecond );
	}

	// After an intentional timing discontinuity (for instance a blocking IO
	// operation) call this to avoid having the fixed timestep logic attempt a
	// set of catch-up Update calls.

	void ResetElapsedTime() noexcept;

	// Update timer state, calling the specified Update function the appropriate
	// number of times.
	void Tick( const std::function<void()>& update );

private:
	// Source timing data uses QPC units.
	uint64_t m_qpcFrequency;
	uint64_t m_qpcLastTime;
	uint64_t m_qpcMaxDelta;

	// Derived timing data uses a canonical tick format.
	uint64_t m_elapsedTicks;
	uint64_t m_totalTicks;
	uint64_t m_leftOverTicks;

	// Members for tracking the framerate.
	uint32_t m_frameCount;
	uint32_t m_framesPerSecond;
	uint32_t m_framesThisSecond;
	uint64_t m_qpcSecondCounter;

	// Members for configuring fixed timestep mode.
	bool m_isFixedTimeStep;
	uint64_t m_targetElapsedTicks;
};

}  // namespace octave

#endif  // OCTAVE_STEPTIMER_HPP
