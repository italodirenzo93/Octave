#ifndef OCTAVE_GRAPHICSSYSTEM_HPP
#define OCTAVE_GRAPHICSSYSTEM_HPP

#include <memory>
#include <string>

#include "Renderer.hpp"

namespace Octave {

class GraphicsSystem {
public:
	GraphicsSystem() = default;
	virtual ~GraphicsSystem() noexcept = default;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<Renderer> CreateRenderer() noexcept = 0;
};

}

#endif
