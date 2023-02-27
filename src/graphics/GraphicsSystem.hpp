#ifndef OCTAVE_GRAPHICSSYSTEM_HPP
#define OCTAVE_GRAPHICSSYSTEM_HPP

#include <memory>
#include <string>

#include "Renderer.hpp"
#include "core/Window.hpp"

namespace Octave {

class GraphicsSystem {
public:
	GraphicsSystem() = default;
	virtual ~GraphicsSystem() noexcept = default;
    
    virtual void SwapBuffers() = 0;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<Renderer> CreateRenderer() noexcept = 0;
    
    static std::unique_ptr<GraphicsSystem> Create( const Window& window );
};

}

#endif
