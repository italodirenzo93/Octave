#ifndef OCTAVE_SAMPLE_HPP
#define OCTAVE_SAMPLE_HPP

#include "Application.hpp"
#include "platform/Window.hpp"
#include "Camera.hpp"
#include "graphics/Renderer.hpp"
#include "helpers/StepTimer.hpp"
#include "pch.hpp"

namespace Octave::Samples {

class Sample : public Application {
protected:
	void Initialize() override;
	void Update() override;

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	DebugCamera camera_;
	std::unique_ptr<StepTimer> step_timer_;
	std::unique_ptr<Window> window_;
	std::unique_ptr<Renderer> renderer_;
};

}  // namespace Octave::Samples

#endif  // OCTAVE_SAMPLE_HPP
