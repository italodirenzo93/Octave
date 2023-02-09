#ifndef OCTAVE_MESHCOMPONENT_HPP
#define OCTAVE_MESHCOMPONENT_HPP

#include "graphics/Mesh.hpp"

namespace octave::scene {

class MeshComponent {
public:
	std::shared_ptr<graphics::Mesh> mesh_;
};

}  // namespace octave::scene

#endif  // OCTAVE_MESHCOMPONENT_HPP
