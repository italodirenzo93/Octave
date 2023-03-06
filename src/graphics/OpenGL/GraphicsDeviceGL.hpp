#ifndef OCTAVE_GRAPHICSDEVICEGL_HPP
#define OCTAVE_GRAPHICSDEVICEGL_HPP

#include "graphics/GraphicsDevice.hpp"

struct GLFWwindow;

namespace Octave::Impl {

class GraphicsDeviceGL : public GraphicsDevice {
public:
	// Constructors
	explicit GraphicsDeviceGL( GLFWwindow* window );
	GraphicsDeviceGL( const GraphicsDeviceGL& other ) = delete;
	GraphicsDeviceGL( GraphicsDeviceGL&& other ) noexcept = delete;
	~GraphicsDeviceGL() noexcept override;

	// Operators
	GraphicsDeviceGL& operator=( const GraphicsDeviceGL& other ) = delete;
	GraphicsDeviceGL& operator=( GraphicsDeviceGL&& other ) noexcept = delete;

	// Methods
    void SwapBuffers() override;

	[[nodiscard]] std::string TryDequeueError() noexcept override;

	[[nodiscard]] Ref<GraphicsContext> CreateContext() noexcept override;
	[[nodiscard]] Ref<Buffer> CreateBuffer( BufferBinding binding, size_t byte_width ) noexcept override;
	[[nodiscard]] Ref<Texture> CreateTexture() noexcept override;

private:
    GLFWwindow* window_;
};

}

#endif

