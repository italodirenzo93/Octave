#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

// clang-format off
#include "input/InputSystem.hpp"
#include "core/Platform.hpp"
#include "graphics/GraphicsDevice.hpp"
#include "LayerStack.hpp"
// clang-format on

namespace Octave {

class Application {
public:
	// Constructors
	Application();
	Application( int argc, char* argv[] );
	virtual ~Application() noexcept;

	// Getter/setter pairs
	[[nodiscard]] Window& GetWindow() const noexcept { return *window_; }
	[[nodiscard]] InputSystem& GetInputSystem() const noexcept { return *input_; }
	[[nodiscard]] GraphicsDevice& GetGraphicsDevice() const noexcept { return *graphics_; }

	// Methods
	void Run();
	void Exit();

protected:
	// Methods
	virtual void OnInitialize() {};
	virtual void OnExit() noexcept {};

	Application& PushLayer( LayerStack::LayerPtr layer ) noexcept;
	LayerStack::LayerPtr PopLayer() noexcept;

private:
	Ref<Window> window_;
	Ref<InputSystem> input_;
	Ref<GraphicsDevice> graphics_;

    LayerStack layers_;

	bool is_running_;

public:
	Application( const Application& ) = delete;
	Application( Application&& ) = delete;
	Application& operator=( const Application& ) = delete;
	Application& operator=( Application&& ) = delete;
};

// To be implemented by the client application
Ref<Application> CreateApplication( int argc, char* argv[] );

}  // namespace Octave

#endif
