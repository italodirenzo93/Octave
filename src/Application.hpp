#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

// clang-format off
#include "input/InputSystem.hpp"
#include "core/Platform.hpp"
#include "graphics/GraphicsSystem.hpp"
#include "LayerStack.hpp"
// clang-format on

namespace Octave {

class Application {
public:
	// Constructors
	Application();
	Application( int argc, char* argv[] );
	virtual ~Application() noexcept = default;

	// Getter/setter pairs
	[[nodiscard]] Window& GetWindow() const noexcept { return *window_; }
	[[nodiscard]] InputSystem& GetInputSystem() const noexcept { return *input_; }
	[[nodiscard]] GraphicsSystem& GetGraphicsSystem() const noexcept { return *graphics_; }

	// Methods
	void Run();
	void Exit();

protected:
	// Methods
	virtual void OnInitialize() {};
	virtual void OnExit() noexcept {};

	Application& PushLayer( LayerStack::LayerPtr layer ) noexcept;
	[[nodiscard]] LayerStack::LayerPtr PopLayer() noexcept;

private:
	std::unique_ptr<Window> window_;
	std::unique_ptr<InputSystem> input_;
	std::unique_ptr<GraphicsSystem> graphics_;
    
    LayerStack layers_;

	bool is_running_;

public:
	Application( const Application& ) = delete;
	Application( Application&& ) = delete;
	Application& operator=( const Application& ) = delete;
	Application& operator=( Application&& ) = delete;
};

// To be implemented by the client application
std::unique_ptr<Application> CreateApplication( int argc, char* argv[] );

}  // namespace Octave

#endif
