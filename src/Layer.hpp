#ifndef OCTAVE_LAYER_HPP
#define OCTAVE_LAYER_HPP

#include <string>

namespace Octave {

class Layer {
public:
	explicit Layer( const std::string& name = "Layer" ) : name_(name) {}
	virtual ~Layer() noexcept = default;

	[[nodiscard]] std::string GetName() const noexcept { return name_; }
	[[nodiscard]] bool IsEnabled() const noexcept { return enabled_; }

	void SetEnabled( bool enabled ) noexcept;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnEnabled() {}
	virtual void OnDisabled() {}
	virtual void OnUpdate() {}
	virtual void OnEvent() {}

protected:
	std::string name_;
	bool enabled_;
};

}

#endif
