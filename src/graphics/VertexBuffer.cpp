#include "pch/pch.hpp"
#include "VertexBuffer.hpp"

namespace Octave {

bool LayoutBinding::operator==( const LayoutBinding& other ) const noexcept {
	return name == other.name && size == other.size &&
		   type == other.type && normalized == other.normalized;
}

}  // namespace Octave
