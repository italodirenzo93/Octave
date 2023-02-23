#ifndef OCTAVE_SAMPLE_HPP
#define OCTAVE_SAMPLE_HPP

#include "Camera.hpp"
#include "CommonInclude.hpp"
#include "StepTimer.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Window.hpp"

namespace octave::samples {

class Sample {
public:
	Sample() = delete;
	explicit Sample( const std::shared_ptr<graphics::Window>& window ) noexcept;
	virtual ~Sample() noexcept = default;

	virtual void OnLoad() {}
	virtual void OnUpdate( const StepTimer& timer ) {}
	virtual void OnRender() {}
	virtual void OnUnload() {}

	void Run();
	void Exit();

protected:
	StepTimer step_timer_;
	DebugCamera camera_;
	std::shared_ptr<graphics::Window> window_;
	graphics::Renderer renderer_;
	bool should_quit_ = false;

private:
	NON_COPYABLE_OR_MOVABLE_CLASS( Sample )
};

}  // namespace octave::samples

#endif  // OCTAVE_SAMPLE_HPP
