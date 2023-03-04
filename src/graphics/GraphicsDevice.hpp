#ifndef OCTAVE_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICSDEVICE_HPP

#include <memory>
#include <string>

#include "core/Window.hpp"
#include "GraphicsContext.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

namespace Octave {

class GraphicsDevice {
public:
	virtual ~GraphicsDevice() noexcept = default;

    virtual void SwapBuffers() = 0;

	[[nodiscard]] virtual std::string TryDequeueError() noexcept = 0;

	[[nodiscard]] virtual std::unique_ptr<GraphicsContext> CreateContext() noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<Buffer> CreateBuffer( BufferBinding binding, size_t byte_width ) noexcept = 0;
	[[nodiscard]] virtual std::unique_ptr<Texture> CreateTexture() noexcept = 0;

    static std::unique_ptr<GraphicsDevice> Create( const Window& window );
};

}

#endif
