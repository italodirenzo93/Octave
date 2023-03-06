#ifndef OCTAVE_GRAPHICSDEVICE_HPP
#define OCTAVE_GRAPHICSDEVICE_HPP

#include <string>

#include "core/Types.hpp"
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

	[[nodiscard]] virtual Ref<GraphicsContext> CreateContext() noexcept = 0;
	[[nodiscard]] virtual Ref<Buffer> CreateBuffer( BufferBinding binding, size_t byte_width ) noexcept = 0;
	[[nodiscard]] virtual Ref<Texture> CreateTexture() noexcept = 0;

    static Ref<GraphicsDevice> Create( const Window& window );
};

}

#endif
