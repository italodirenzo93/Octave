#ifndef OCTAVE_GRAPHICSSYSTEM_HPP
#define OCTAVE_GRAPHICSSYSTEM_HPP

#include <memory>
#include <string>

#include "core/Window.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

namespace Octave {

class GraphicsSystem {
public:
	GraphicsSystem() = default;
	virtual ~GraphicsSystem() noexcept = default;

    virtual void SwapBuffers() = 0;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<Renderer> CreateRenderer() noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<Buffer> CreateBuffer( BufferBinding binding, size_t byteWidth ) noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<Texture> CreateTexture() noexcept = 0;

    static std::unique_ptr<GraphicsSystem> Create( const Window& window );
};

}

#endif
