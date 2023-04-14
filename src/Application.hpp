#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

// clang-format off
#include "input/InputSystem.hpp"
#include "core/Platform.hpp"
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
	[[nodiscard]] Window& GetWindow() const noexcept { return *m_Window; }
	[[nodiscard]] InputSystem& GetInputSystem() const noexcept { return *m_Input; }
	[[nodiscard]] GraphicsDevice& GetGraphicsDevice() const noexcept { return m_Window->GetGraphicsDevice(); }

	// Methods
	void Run();
	void Exit();

protected:
	// Methods
	virtual void OnInitialize() {}
	virtual void OnUpdate() {}
	virtual void OnExit() noexcept {}

	Application& PushLayer( LayerStack::LayerPtr layer ) noexcept;
	LayerStack::LayerPtr PopLayer() noexcept;

private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<InputSystem> m_Input;

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
