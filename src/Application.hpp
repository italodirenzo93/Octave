#ifndef OCTAVE_APPLICATION_HPP
#define OCTAVE_APPLICATION_HPP

#include <memory>

#include "platform/Platform.hpp"

namespace octave {

class Application {
public:
	// Constructors
	Application();
	Application( const Application& other ) = delete;
	Application( Application&& other ) noexcept = delete;
	virtual ~Application() noexcept;

	// Operators
	Application& operator=( const Application& other ) = delete;
	Application& operator=( Application&& other ) noexcept = delete;

	// Getter/setter pairs
	[[nodiscard]] platform::Platform& GetPlatform() const noexcept { return *platform_; }

	// Methods
	void Run();
	void Exit();

	// Fields

protected:
	// Methods
	virtual void Initialize();
	virtual void Update();
	virtual void Render();

	// Fields
	std::unique_ptr<platform::Platform> platform_;

private:
	// Methods

	// Fields
	bool is_running_;
};

}  // namespace octave

#endif
