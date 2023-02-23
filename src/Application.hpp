#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

#include <memory>

#include "platform/Platform.hpp"

namespace Octave {

class Application {
public:
	// Constructors
	Application( int argc, char* argv[] );
	Application( const Application& other ) = delete;
	Application( Application&& other ) noexcept = delete;
	virtual ~Application() noexcept;

	// Operators
	Application& operator=( const Application& other ) = delete;
	Application& operator=( Application&& other ) noexcept = delete;

	// Getter/setter pairs
	[[nodiscard]] Platform& GetPlatform() const noexcept { return *platform_; }

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

private:
	// Methods

	// Fields
	bool is_running_;
};

}  // namespace octave

#endif
