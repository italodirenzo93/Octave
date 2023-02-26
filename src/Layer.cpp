#include "pch/pch.hpp"
#include "Layer.hpp"

namespace Octave {

void Layer::SetEnabled( bool enabled ) noexcept {
	if ( enabled ) {
		OnEnabled();
	} else {
		OnDisabled();
	}

	enabled_ = enabled;
}

}
