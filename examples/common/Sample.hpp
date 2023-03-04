#pragma once

#include <Octave.hpp>

class Sample : public Octave::Application {
protected:
	void OnInitialize() override;
};

class SampleLayer : public Octave::Layer {
public:
	SampleLayer( const Sample& app );

	virtual void OnStep() = 0;
	virtual void OnRender() = 0;

protected:
	Octave::DebugCamera camera_;
	std::unique_ptr<Octave::StepTimer> step_timer_;
	std::unique_ptr<Octave::GraphicsContext> renderer_;
};

