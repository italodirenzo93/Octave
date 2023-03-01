#ifndef OCTAVE_GRAPHICSSYSTEM_HPP
#define OCTAVE_GRAPHICSSYSTEM_HPP

#include <memory>
#include <string>

#include "core/Window.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Octave {

class GraphicsSystem {
public:
	GraphicsSystem() = default;
	virtual ~GraphicsSystem() noexcept = default;

    virtual void SwapBuffers() = 0;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<Renderer> CreateRenderer() noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<VertexBuffer> CreateVertexBuffer() noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<IndexBuffer> CreateIndexBuffer() noexcept = 0;

    static std::unique_ptr<GraphicsSystem> Create( const Window& window );
};

}

#endif
