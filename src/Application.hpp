#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

// clang-format off
#include "input/InputSystem.hpp"
#include "platform/Platform.hpp"
// clang-format on

namespace Octave {

class Application {
public:
	// Constructors
	Application();
	Application( int argc, char* argv[] );
	virtual ~Application() noexcept;

	// Getter/setter pairs
	[[nodiscard]] Platform& GetPlatform() const noexcept { return *platform_; }
	[[nodiscard]] InputSystem& GetInput() const noexcept { return *input_; }

	// Methods
	void Run();
	void Exit();

	// Fields

protected:
	// Methods
	virtual void Initialize();
	virtual void Update();

	// Fields
	std::unique_ptr<Platform> platform_;
	std::unique_ptr<InputSystem> input_;

private:
	// Methods

	// Fields
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
