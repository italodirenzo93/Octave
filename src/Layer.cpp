#include "pch/pch.hpp"
#include "Layer.hpp"

namespace Octave {

Layer::Layer( std::string name ) noexcept
	: name_( std::move( name ) ), enabled_( true ) {
}


void Layer::SetEnabled( bool enabled ) noexcept {
	if ( enabled ) {
		OnEnabled();
	} else {
		OnDisabled();
	}

	enabled_ = enabled;
}

}  // namespace Octave
