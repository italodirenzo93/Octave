#pragma once

#include <Octave.hpp>

class Sample : public Octave::Application {
protected:
	void Initialize() override;
	void Update() override;

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	Octave::DebugCamera camera_;
	std::unique_ptr<Octave::StepTimer> step_timer_;
	std::unique_ptr<Octave::Window> window_;
	std::unique_ptr<Octave::Renderer> renderer_;
};
